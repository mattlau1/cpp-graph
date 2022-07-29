#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>
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
		graph() noexcept = default;

		graph(std::initializer_list<N> il)
		: graph_{std::unordered_map<N, edge>{}} {
			for (auto const& i : il) {
				graph_.emplace(i, edge{});
			}
		}

		template<typename InputIt>
		graph(InputIt first, InputIt last)
		: graph_{std::unordered_map<N, edge>{}} {
			for (auto const& f = first; f != last; ++first) {
				graph_.emplace(*f, edge{});
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
		: graph_{std::unordered_map<N, edge>{other.graph_}} {};

		// TODO(implement)
		// auto operator=(graph const& other) -> graph& {
		// 	auto copy = other;
		// 	std::swap(copy, *this);
		// 	return *this;
		// };

		auto insert_node(N const& value) -> bool {
			// return graph_.emplace(value, edge{}).second;
			if (is_node(value)) {
				return false;
			}
			graph_.emplace(value, edge{});
			return true;
		};

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
				return false;
			}
			// TODO(check if edge was actually added)
			graph_.at(src).push_back(make_pair(dst, weight));
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
				if (conn.first != old_data) {
					// replace reference to old_data in conn with new_data
					for (auto const& e : graph_.at(conn)) {
						if (e.first == old_data) {
							e.first = new_data;
							break;
						}
					}
				}
				else {
					// update key
					auto item = graph_.extract(conn);
					item.key() = new_data;
					graph_.insert(std::move(item));
				}
			}
		}

		// TODO(implement)
		// auto merge_replace_node(N const& old_data, N const& new_data) -> void;

		auto erase_node(N const& value) -> bool {
			if (is_node(value)) {
				return false;
			}
			for (auto const& conn : graph_) {
				for (auto const& e : conn) {
					if (e.first == value) {
						conn.second.erase(e);
					}
				}
			}
			return true;
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
				return false;
			}
			for (auto const& conn : graph_.at(src)) {
				if (conn.first == dst && conn.second == weight) {
					graph_.at(src).erase(conn);
					break;
				}
			}
			return true;
		}

		// TODO(implement)
		// auto erase_edge(iterator i) -> iterator;
		// auto erase_edge(iterator i, iterator s) -> iterator;

		auto clear() noexcept -> void {
			graph_.clear();
		}

		[[nodiscard]] auto is_node(N const& value) -> bool {
			return graph_.contains(value);
		}

		[[nodiscard]] auto empty() -> bool {
			return graph_.empty();
		}

		[[nodiscard]] auto empty() const -> bool {
			return graph_.empty();
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool {
			return static_cast<bool>(std::find_if(graph_.at(src).begin(),
			                                      graph_.at(src).end(),
			                                      [&dst](auto const& e) { return e.first == dst; }));
		}

		[[nodiscard]] auto nodes() -> std::vector<N> {
			auto res = std::vector<N>{};
			res.reserve(graph_.size());
			for (auto const& kv : graph_) {
				res.push_back(kv.first);
			}
			std::sort(res.begin(), res.end());
			return res;
		}

		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto res = std::vector<N>{};
			res.reserve(graph_.size());
			for (auto const& kv : graph_) {
				res.push_back(kv.first);
			}
			std::sort(res.begin(), res.end());
			return res;
		}

		[[nodiscard]] auto weights(N const& src, N const& dst) -> std::vector<E> {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
				return nullptr;
			}
			auto res = std::vector<E>{};
			for (auto const& conn : graph_.at(src)) {
				if (conn.first == dst) {
					res.push_back(conn.second);
				}
			}
			std::sort(res.begin(), res.end());
			return res;
		}

		// TODO(implement)
		// [[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator;

		[[nodiscard]] auto connections(N const& src) -> std::vector<N> {
			auto res = std::vector<N>{};
			for (auto const& conn : graph_.at(src)) {
				res.push_back(conn.first);
			}
			// TODO(sort in asc order with respect to connected nodes)
			res.sort();
			return res;
		}

		// TODO(implement):
		// [[nodiscard]] auto begin() const -> iterator;
		// [[nodiscard]] auto end() const -> iterator;

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
				for (auto const& e : g.graph_.at(src)) {
					// e.first == dst, e.second == weight
					os << "  " << e.first << " | " << e.second << "\n";
				}
				os << ")\n";
			}

			return os;
		}

	private:
		// internal graph adjacency list representation
		// {
		//  src: [<dest, weight>]
		// }
		// src ---weight--> dest
		using edge = std::vector<std::pair<N, E>>;
		std::unordered_map<N, edge> graph_;
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
		auto operator*() -> reference{};
		// auto operator->() -> pointer not required

		// Iterator traversal
		auto operator++() -> iterator&;
		auto operator++(int) -> iterator;
		auto operator--() -> iterator&;
		auto operator--(int) -> iterator;

		// Iterator comparison
		auto operator==(iterator const& other) -> bool;

	private:
		using graph_map_iter = typename std::unordered_map<N, edge>::const_iterator;
		using edge_iter = typename edge::const_iterator;
		graph_map_iter graph_map_iter_;
		edge_iter edge_iter_;
		// TODO(implement)
		// explicit iterator(unspecified);
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
