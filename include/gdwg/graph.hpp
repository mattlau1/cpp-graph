#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>

// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		class iterator;
		struct value_type {
			N from;
			N to;
			E weight;
		};

		// TODO(x): check if all functions need const, check constructor implementation and check for
		// STL algorithm replacements. check iterator invalidation and conditions for all fns
		// check noexcepts
		graph() = default;

		graph(std::initializer_list<N> il)
		: graph_{graph_container{}} {
			for (auto const& i : il) {
				graph_.emplace(i, edge_set{});
			}
		}

		template<typename InputIt>
		graph(InputIt first, InputIt last)
		: graph_{graph_container{}} {
			for (auto const& f = first; f != last; ++first) {
				graph_.emplace(*f, edge_set{});
			}
		}

		graph(graph&& other) noexcept
		: graph_{std::exchange(other.graph_, nullptr)} {};

		auto operator=(graph&& other) noexcept -> graph& {
			std::swap(graph_, other.graph_);
			other.graph_ = nullptr;
			return *this;
		};

		graph(graph const& other)
		: graph_{graph_container{}} {
			for (auto const& node : other.nodes()) {
				insert_node(node);
			}

			for (auto const& i : other.graph_) {
				for (auto const& edge : i.second) {
					insert_edge(*i.first, edge->first, edge->second);
				}
			}
		};

		auto operator=(graph const& other) -> graph& {
			if (this != other) {
				auto copy = other;
				std::swap(copy, *this);
			}
			return *this;
		};

		auto insert_node(N const& value) -> bool {
			if (is_node(value)) {
				return false;
			}
			graph_.emplace(std::make_unique<N>(value), edge_set{});
			return true;
		};

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
				return false;
			}
			// TODO(check if edge was actually added)
			// get src node iterator
			auto const& src_iter = get_connections(src);

			// src node not found
			if (src_iter == graph_.end()) {
				return false;
			}

			// look for duplicate connection
			if (std::find_if(src_iter->second.begin(),
			                 src_iter->second.end(),
			                 [&dst, &weight](auto const& edge) {
				                 return edge->first == dst && edge->second == weight;
			                 })
			    != src_iter->second.end())
			{
				return false;
			}

			// graph[src] = unique_ptr(<dst, weight>)
			src_iter->second.emplace(std::make_unique<std::pair<N, E>>(std::make_pair(dst, weight)));
			return true;
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
				return false;
			}
			if (!is_node(new_data)) {
				return false;
			}

			for (auto const& conn : graph_) {
				if (conn.first.get() != old_data) {
					// replace reference to old_data in conn with new_data
					for (auto const& edge : get_connections(conn)->second) {
						if (edge->first == old_data) {
							edge->first = new_data;
							break;
						}
					}
				}
				else {
					// update key
					auto item = graph_.extract(conn);
					item.key() = std::make_unique(new_data);
					graph_.insert(std::move(item));
				}
			}
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!is_node(old_data) || !is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}

			for (auto const& conn : graph_) {
				if (conn->first == old_data) {
					for (auto const& edge : conn.second) {
						if (edge->first != old_data) {
							insert_edge(new_data, edge->first, edge->weight);
						}
						else {
							insert_edge(new_data, new_data, edge->weight);
						}
					}
				}
				else {
					for (auto const& edge : conn.second) {
						if (edge->first == old_data) {
							insert_edge(conn->first, new_data, edge->weight);
						}
					}
				}
			}
			erase_node(old_data);
		}

		auto erase_node(N const& value) -> bool {
			if (is_node(value)) {
				return false;
			}
			for (auto const& conn : graph_) {
				std::erase_if(conn.second, [&value](auto const& edge) { return edge->first == value; });
			}
			graph_.erase(get_connections(value)->first);
			return true;
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
				return false;
			}
			std::erase_if(get_connections(src)->second, [&dst, &weight](auto const& edge) {
				return edge->first == dst && edge->second == weight;
			});
			return true;
		}

		auto erase_edge(iterator i) -> iterator;
		auto erase_edge(iterator i, iterator s) -> iterator;

		auto clear() noexcept -> void {
			graph_.clear();
		}

		[[nodiscard]] auto is_node(N const& value) -> bool {
			return get_connections(value) != graph_.end();
		}

		[[nodiscard]] auto empty() -> bool {
			return graph_.empty();
		}

		[[nodiscard]] auto empty() const -> bool {
			return graph_.empty();
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node don't exist in the graph");
			}
			auto const& src_edges = get_connections(src)->second;
			return std::find_if(src_edges.begin(), src_edges.end(), [&dst](auto const& edge) {
				return edge->first == dst;
			});
		}

		[[nodiscard]] auto nodes() -> std::vector<N> {
			auto res = std::vector<N>{};
			res.reserve(graph_.size());
			for (auto const& kv : graph_) {
				res.push_back(kv.first.get());
			}
			return res;
		}

		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto res = std::vector<N>{};
			res.reserve(graph_.size());
			for (auto const& kv : graph_) {
				res.push_back(*kv.first);
			}
			return res;
		}

		[[nodiscard]] auto weights(N const& src, N const& dst) -> std::vector<E> {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
				return nullptr;
			}
			auto res = std::vector<E>{};
			for (auto const& conn : get_connections(src)->second) {
				if (conn->first == dst) {
					res.push_back(conn->second);
				}
			}
			return res;
		}

		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator {
			auto it = begin();
			auto const& it_end = end();
			while (it != it_end) {
				auto const& [curr_src, curr_dst, curr_weight] = *it;
				if (curr_src == src && curr_dst == dst && curr_weight == weight) {
					return it;
				}
				++it;
			}
			return it;
		};

		[[nodiscard]] auto connections(N const& src) -> std::vector<N> {
			auto res = std::vector<N>{};
			for (auto const& conn : get_connections(src)->second) {
				res.push_back(conn->first);
			}
			return res;
		}

		[[nodiscard]] auto begin() const -> iterator {
			return iterator(graph_.begin(), graph_.end(), (graph_.begin())->second.begin());
		}

		[[nodiscard]] auto end() const -> iterator {
			return iterator(graph_.end(), graph_.end(), {});
		}

		// TODO(use find())
		[[nodiscard]] auto operator==(graph const& other) -> bool {
			auto const& this_nodes = nodes();
			auto const& other_nodes = other.nodes();
			if (empty() != other.empty() || this_nodes != other_nodes) {
				return false;
			}
			for (auto const& node : this_nodes) {
				auto const& this_conns = connections(node);
				auto const& other_conns = other.connections(node);
				if (this_conns != other_conns) {
					return false;
				}
				for (auto const& conn : this_conns) {
					if (weights(node, conn) != other.weights(node, conn)) {
						return false;
					}
				}
			}
			return true;
		}

		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			if (g.empty()) {
				os << "";
				return os;
			}

			for (auto const& src : g.nodes()) {
				os << src << " ";
				os << "(\n";
				for (auto const& edge : g.get_connections(src)->second) {
					os << "  " << edge->first << " | " << edge->second << "\n";
				}
				os << ")\n";
			}

			return os;
		}

	private:
		struct graph_map_comparator {
			auto operator()(std::unique_ptr<N> const& lhs, std::unique_ptr<N> const& rhs) const -> bool {
				return *lhs < *rhs;
			};
		};
		struct edge_set_comparator {
			auto operator()(std::unique_ptr<std::pair<N, E>> const& lhs,
			                std::unique_ptr<std::pair<N, E>> const& rhs) const -> bool {
				if (lhs->first != rhs->first) {
					return lhs->first < rhs->first;
				}
				return lhs->second < rhs->second;
			}
		};

		using edge = std::unique_ptr<std::pair<N, E>>;
		using edge_set = std::set<edge, edge_set_comparator>;
		using node = std::unique_ptr<N>;
		using graph_container = std::map<node, edge_set, graph_map_comparator>;
		graph_container graph_;

		// returns an iterator to a <node, edge_set> pair in the graph given a source node else
		// graph_.end() if node not in graph
		auto get_connections(N node) -> typename graph_container::iterator {
			return std::find_if(graph_.begin(), graph_.end(), [&node](auto const& n) {
				return *n.first == node;
			});
		}

		auto get_connections(N node) const -> typename graph_container::const_iterator {
			return std::find_if(graph_.begin(), graph_.end(), [&node](auto const& n) {
				return *n.first == node;
			});
		}
	};

	template<typename N, typename E>
	class graph<N, E>::iterator {
	public:
		using value_type = graph<N, E>::value_type;
		using reference = value_type;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		// Iterator constructor
		iterator() = default;

		// Iterator source
		auto operator*() -> reference {
			return value_type{*graph_iter_->first, (*edge_iter_)->first, (*edge_iter_)->second};
		};

		// auto operator->() -> pointer not required

		// Iterator traversal
		auto operator++() -> iterator& {
			if (edge_iter_ != graph_iter_->second.end()) {
				++edge_iter_;
			}

			if (edge_iter_ == graph_iter_->second.end()) {
				++graph_iter_;
				if (graph_iter_ != graph_iter_end_) {
					while (graph_iter_ != graph_iter_end_ && graph_iter_->second.empty()) {
						++graph_iter_;
					}
					if (graph_iter_ != graph_iter_end_) {
						edge_iter_ = graph_iter_->second.begin();
					}
				}
			}
			return *this;
		}

		auto operator++(int) -> iterator {
			auto copy{*this};
			++(*this);
			return copy;
		}

		auto operator--() -> iterator& {
			while (graph_iter_ == graph_iter_end_ || graph_iter_->second.empty()) {
				--graph_iter_;
				edge_iter_ = graph_iter_->second.end();
			}

			if (edge_iter_ == graph_iter_->second.begin()) {
				--graph_iter_;
				while (graph_iter_ == graph_iter_end_ || graph_iter_->second.empty()) {
					--graph_iter_;
				}
				edge_iter_ = graph_iter_->second.end();
			}

			--edge_iter_;
			return *this;
		}

		auto operator--(int) -> iterator {
			auto copy{*this};
			--(*this);
			return copy;
		}

		// Iterator comparison
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

		explicit iterator(map_iter graph_iter_begin, map_iter graph_iter_end, edges_iter edge_iter)
		: graph_iter_{graph_iter_begin}
		, graph_iter_end_{graph_iter_end}
		, edge_iter_{edge_iter} {
			if (graph_iter_begin != graph_iter_end) {
				while (graph_iter_ != graph_iter_end_ && graph_iter_->second.empty()) {
					++graph_iter_;
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
		auto const& [to, from, weight] = *i;
		erase_edge(to, from, weight);
		return next;
	};

	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i, iterator s) -> iterator {
		while (i != s) {
			i = erase_edge(i);
		}
		return i;
	}

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
