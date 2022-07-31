#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		// --------------------------------------------
		// Iterator [gdwg.iterator]
		// --------------------------------------------
		class iterator;

		struct value_type {
			N from;
			N to;
			E weight;
		};

		// --------------------------------------------
		// Constructors [gdwg.ctor]
		// --------------------------------------------

		graph() noexcept = default;

		graph(std::initializer_list<N> il)
		: graph_{graph_container{}} {
			std::for_each(il.begin(), il.end(), [this](auto const& i) { insert_node(i); });
		}

		template<typename InputIt>
		graph(InputIt first, InputIt last)
		: graph_{graph_container{}} {
			std::for_each(first, last, [this](auto const& i) { insert_node(i); });
		}

		graph(graph&& other) noexcept
		: graph_{std::exchange(other.graph_, graph_container{})} {};

		auto operator=(graph&& other) noexcept -> graph& {
			std::swap(graph_, other.graph_);
			other.graph_ = graph_container{};
			return *this;
		};

		graph(graph const& other)
		: graph_{graph_container{}} {
			auto const& other_nodes = other.nodes();
			for (auto const& node : other_nodes) {
				insert_node(node);
			}

			for (auto const& conn : other.graph_) {
				for (auto const& edge : conn.second) {
					insert_edge(*conn.first, edge->first, edge->second);
				}
			}
		};

		auto operator=(graph const& other) -> graph& {
			if (this != &other) {
				auto copy = other;
				std::swap(copy, *this);
			}
			return *this;
		};

		// --------------------------------------------
		// Modifiers [gdwg.modifiers]
		// --------------------------------------------

		// Adds a new node with value value to the graph if, and only if, there is no node equivalent
		// to value already stored.
		auto insert_node(N const& value) -> bool {
			if (is_node(value)) {
				return false;
			}
			graph_.emplace(std::make_unique<N>(value), edge_set{});
			return true;
		};

		// Adds a new edge representing src → dst with weight weight, if, and only if, there is no
		// edge equivalent to value_type{src, dst, weight} already stored.
		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
			}

			auto const& src_iter = graph_.find(src);

			// src node not found
			if (src_iter == graph_.end()) {
				return false;
			}

			// look for duplicate connection
			if (src_iter->second.find(std::make_pair(dst, weight)) != src_iter->second.end()) {
				return false;
			}

			// TODO(check if edge was actually added)
			// graph[src] = unique_ptr(<dst, weight>)
			src_iter->second.emplace(std::make_unique<edge>(std::make_pair(dst, weight)));
			return true;
		}

		// Replaces the original data, old_data, stored at this particular node by the replacement
		// data, new_data. Does nothing if new_data already exists as a node.
		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}

			if (is_node(new_data)) {
				return false;
			}

			for (auto const& conn : graph_) {
				if (*conn.first != old_data) {
					// replace reference to old_data in conn with new_data
					auto const& edges = graph_.find(*conn.first)->second;
					(**edges.find(old_data)).first = new_data;
				}
				else {
					// update key
					auto item = graph_.extract(conn.first);
					item.key() = std::make_unique<N>(new_data);
					graph_.insert(std::move(item));
				}
			}
			return true;
		}

		// The node equivalent to old_data in the graph are replaced with instances of new_data. After
		// completing, every incoming and outgoing edge of old_data becomes an incoming/ougoing edge
		// of new_data, except that duplicate edges shall be removed.
		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!is_node(old_data) || !is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}

			for (auto const& conn : graph_) {
				if (*conn.first == old_data) {
					for (auto const& edge : conn.second) {
						edge->first != old_data ? insert_edge(new_data, edge->first, edge->second)
						                        : insert_edge(new_data, new_data, edge->second);
					}
				}
				else {
					for (auto const& edge : conn.second) {
						if (edge->first == old_data) {
							insert_edge(*conn.first, new_data, edge->second);
						}
					}
				}
			}
			erase_node(old_data);
		}

		// Erases all nodes equivalent to value, including all incoming and outgoing edges.
		auto erase_node(N const& value) -> bool {
			if (!is_node(value)) {
				return false;
			}

			for (auto& conn : graph_) {
				std::erase_if(conn.second, [&value](auto const& edge) { return edge->first == value; });
			}

			graph_.erase(graph_.find(value)->first);
			return true;
		}

		// Erases an edge representing src → dst with weight weight.
		// Complexity: O(log (n) + e), where n is the total number of stored nodes and e is the total
		// number of stored edges.
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
			}

			if (!is_connected(src, dst)) {
				return false;
			}

			auto const& src_node = graph_.find(src);
			auto& src_node_edges = src_node->second;
			auto const& edge_iter = src_node_edges.find(std::make_pair(dst, weight));

			// edge doesn't exist
			if (edge_iter == src_node_edges.end()) {
				return false;
			}

			src_node_edges.erase(edge_iter);
			return true;
		}

		// Erases the edge pointed to by i.
		// Complexity: Amortised constant time.
		auto erase_edge(iterator i) -> iterator;

		// Erases all edges between the iterators [i, s).
		// Complexity O(d), where d=std::distance(i, s).
		auto erase_edge(iterator i, iterator s) -> iterator;

		// Erases all nodes from the graph.
		auto clear() noexcept -> void {
			graph_.clear();
		}

		// --------------------------------------------
		// Accessors [gdwg.accessors]
		// --------------------------------------------

		// Returns: true if a node equivalent to value exists in the graph, and false otherwise.
		// Complexity: O(log (n)) time.
		[[nodiscard]] auto is_node(N const& value) const -> bool {
			return graph_.find(value) != graph_.end();
		}

		// Returns: true if there are no nodes in the graph, and false otherwise.
		[[nodiscard]] auto empty() const -> bool {
			return graph_.empty();
		}

		// Returns: true if an edge src → dst exists in the graph, and false otherwise.
		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node don't exist in the graph");
			}
			auto const& src_edges = graph_.find(src)->second;
			return src_edges.find(dst) != src_edges.end();
		}

		// Returns: A sequence of all stored nodes, sorted in ascending order.
		// Complexity: O(n), where n is the number of stored nodes.
		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto res = std::vector<N>{};
			res.reserve(graph_.size());
			for (auto const& conn : graph_) {
				res.push_back(*conn.first);
			}
			return res;
		}

		// Returns: A sequence of weights from src to dst, sorted in ascending order.
		// Complexity: O(log (n) + e), where n is the number of stored nodes and e is the number of
		// stored edges.
		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
			}
			auto res = std::vector<E>{};
			for (auto const& conn : graph_.find(src)->second) {
				if (conn->first == dst) {
					res.push_back(conn->second);
				}
			}
			return res;
		}

		// Returns: An iterator pointing to an edge equivalent to value_type{src, dst, weight}, or
		// end() if no such edge exists.
		// Complexity: O(log (n) + log (e)), where n is the number of stored nodes and e is the number
		// of stored edges.
		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			// log(n)
			auto const& graph_iter = graph_.find(src);
			if (graph_iter == graph_.end()) {
				return end();
			}

			// log(e)
			auto edge_iter = graph_iter->second.find(std::make_pair(dst, weight));
			if (edge_iter == graph_iter->second.end()) {
				return end();
			}
			return iterator(graph_iter, graph_.end(), edge_iter);
		};

		// Returns: A sequence of nodes (found from any immediate outgoing edge) connected to src,
		// sorted in ascending order, with respect to the connected nodes.
		// Complexity: O(log (n) + e), where e is the number of outgoing edges associated with src.
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			auto res = std::set<N>{};
			for (auto const& conn : graph_.find(src)->second) {
				res.emplace(conn->first);
			}
			return std::vector<N>(res.begin(), res.end());
		}

		// --------------------------------------------
		// Iterator access [gdwg.iterator.access]
		// --------------------------------------------

		// Returns: An iterator pointing to the first element in the container.
		[[nodiscard]] auto begin() const -> iterator {
			return iterator(graph_.begin(), graph_.end(), {});
		}

		// Returns: An iterator denoting the end of the iterable list that begin() points to.
		[[nodiscard]] auto end() const -> iterator {
			return iterator(graph_.end(), graph_.end(), {});
		}

		// --------------------------------------------
		// Comparisons [gdwg.cmp]
		// --------------------------------------------

		// Returns: true if *this and other contain exactly the same nodes and edges, and false
		// otherwise.
		// Complexity: O(n + e) where n is the sum of stored nodes in *this and other, and e is the
		// sum of stored edges in *this and other.
		// TODO(fix complexity)
		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			// O(n)
			auto const& this_nodes = nodes();
			auto const& other_nodes = other.nodes();

			if (empty() != other.empty() || this_nodes != other_nodes) {
				return false;
			}

			// O(n)
			for (auto const& node : this_nodes) {
				// O(log(n) + e) => O(n + e)
				auto const& this_conns = connections(node);
				auto const& other_conns = other.connections(node);
				if (this_conns != other_conns) {
					return false;
				}
				// O(e)
				for (auto const& conn : this_conns) {
					// O(n + e)
					if (weights(node, conn) != other.weights(node, conn)) {
						return false;
					}
				}
			}
			return true;
		}

		// --------------------------------------------
		// Extractor [gdwg.io]
		// --------------------------------------------

		// Behaves as a formatted output function of os.
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			if (g.empty()) {
				os << "";
				return os;
			}

			for (auto const& src : g.nodes()) {
				os << src << " ";
				os << "(\n";
				for (auto const& edge : g.graph_.find(src)->second) {
					os << "  " << edge->first << " | " << edge->second << "\n";
				}
				os << ")\n";
			}

			return os;
		}

	private:
		using node_ptr = std::unique_ptr<N>;
		using edge = std::pair<N, E>;
		using edge_ptr = std::unique_ptr<edge>;

		struct graph_map_comparator {
			using is_transparent = void;
			auto operator()(node_ptr const& lhs, node_ptr const& rhs) const -> bool {
				return *lhs < *rhs;
			}

			auto operator()(node_ptr const& lhs, N const& rhs) const noexcept -> bool {
				return *lhs < rhs;
			}

			auto operator()(N const& lhs, node_ptr const& rhs) const noexcept -> bool {
				return lhs < *rhs;
			}
		};

		struct edge_set_comparator {
			using is_transparent = void;
			auto operator()(edge_ptr const& lhs, edge_ptr const& rhs) const -> bool {
				if (lhs->first != rhs->first) {
					return lhs->first < rhs->first;
				}
				return lhs->second < rhs->second;
			}
			auto operator()(const edge_ptr& lhs, const edge& rhs) const -> bool {
				if (lhs->first != rhs.first) {
					return lhs->first < rhs.first;
				}
				return lhs->second < rhs.second;
			}
			auto operator()(const edge& lhs, const edge_ptr& rhs) const -> bool {
				if (lhs.first != rhs->first) {
					return lhs.first < rhs->first;
				}
				return lhs.second < rhs->second;
			}
			auto operator()(N lhs, const edge_ptr& rhs) const -> bool {
				return lhs < rhs->first;
			}
			auto operator()(const edge_ptr& lhs, N rhs) const -> bool {
				return lhs->first < rhs;
			}
		};

		using edge_set = std::set<edge_ptr, edge_set_comparator>;
		using graph_container = std::map<node_ptr, edge_set, graph_map_comparator>;
		graph_container graph_;
	};

	template<typename N, typename E>
	class graph<N, E>::iterator {
	public:
		using value_type = graph<N, E>::value_type;
		using reference = value_type;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		// --------------------------------------------
		// Iterator constructor [gdwg.iterator.ctor]
		// --------------------------------------------

		// Value-initialises all members.
		iterator() = default;

		// --------------------------------------------
		// Iterator source [gdwg.iterator.source]
		// --------------------------------------------

		// Returns the current from, to, and weight.
		auto operator*() const -> reference {
			return value_type{*graph_iter_->first, (*edge_iter_)->first, (*edge_iter_)->second};
		};

		// auto operator->() -> pointer not required

		// --------------------------------------------
		// Iterator traversal [gdwg.iterator.traversal]
		// --------------------------------------------

		// Advances *this to the next element in the iterable list.
		auto operator++() -> iterator& {
			if (edge_iter_ != graph_iter_->second.end()) {
				++edge_iter_;
			}

			if (edge_iter_ == graph_iter_->second.end()) {
				++graph_iter_;
				if (graph_iter_ != graph_iter_end_) {
					for (; graph_iter_ != graph_iter_end_ && graph_iter_->second.empty(); ++graph_iter_) {
					}
					if (graph_iter_ != graph_iter_end_) {
						edge_iter_ = graph_iter_->second.begin();
					}
				}
			}
			return *this;
		}

		// Advances *this to the next element in the iterable list and returns *this.
		auto operator++(int) -> iterator {
			auto temp = *this;
			++*this;
			return temp;
		}

		// Advances *this to the previous element in the iterable list.
		auto operator--() -> iterator& {
			while (graph_iter_ == graph_iter_end_ || graph_iter_->second.empty()) {
				--graph_iter_;
				edge_iter_ = graph_iter_->second.end();
			}

			if (edge_iter_ == graph_iter_->second.begin()) {
				--graph_iter_;
				for (; graph_iter_ == graph_iter_end_ || graph_iter_->second.empty(); --graph_iter_) {
				}
				edge_iter_ = graph_iter_->second.end();
			}

			--edge_iter_;
			return *this;
		}

		// Advances *this to the previous element in the iterable list and returns *this.
		auto operator--(int) -> iterator {
			auto temp = *this;
			--*this;
			return temp;
		}

		// --------------------------------------------
		// Iterator comparison [gdwg.iterator.comparison]
		// --------------------------------------------

		// Returns: true if *this and other are pointing to the same elements in the same iterable
		// list, and false otherwise.
		auto operator==(iterator const& other) const -> bool {
			if (graph_iter_ == graph_iter_end_ || other.graph_iter_ == other.graph_iter_end_) {
				return graph_iter_ == other.graph_iter_;
			}
			return graph_iter_ == other.graph_iter_ && edge_iter_ == other.edge_iter_;
		}

	private:
		using map_iter = typename graph_container::const_iterator;
		using edges_iter = typename edge_set::const_iterator;
		map_iter graph_iter_;
		map_iter graph_iter_end_;
		edges_iter edge_iter_;

		// --------------------------------------------
		// Iterator constructor [gdwg.iterator.ctor]
		// --------------------------------------------

		// Constructs an iterator to a specific element in the graph.
		explicit iterator(map_iter graph_iter_begin, map_iter graph_iter_end, edges_iter edge_iter)
		: graph_iter_{graph_iter_begin}
		, graph_iter_end_{graph_iter_end}
		, edge_iter_{edge_iter} {
			if (graph_iter_begin != graph_iter_end) {
				for (; graph_iter_ != graph_iter_end_ && graph_iter_->second.empty(); ++graph_iter_) {
				}
				if (graph_iter_ != graph_iter_end_) {
					edge_iter_ = graph_iter_->second.begin();
				}
			}
		};

		friend class graph;
	};

	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i) -> iterator {
		auto next = i;
		++next;
		auto const& edge_iter = i.edge_iter_;
		auto const& src_node = (**edge_iter).first;
		auto& edge_set = (graph_.find(src_node))->second;
		edge_set.erase(edge_iter);
		return next;
	};

	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i, iterator s) -> iterator {
		for (; i != s; i = erase_edge(i)) {
		};
		return i;
	}

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
