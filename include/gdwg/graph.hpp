#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>

// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
	private:
		// internal graph adjacency list representation
		// {
		//  src: [<dest, weight>]
		// }
		// src ---weight--> dest
		using edge = std::unordered_set<std::pair<N, E>>;
		std::unordered_map<N, edge> graph_;

	public:
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
		: graph_{std::unordered_map<N, edge>{other.graph_}} {
			std::copy(other.graph_.begin(), other.graph_.end(), graph_.begin());
		};

		// auto operator=(graph const& other) -> graph& {
		// 	auto copy = other;
		// 	std::swap(copy, *this);
		// 	return *this;
		// };

		auto insert_node(N const& value) -> bool {
			// return graph_.emplace(value, edge{}).second;
			if (!is_node(value)) {
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
			// TODO(): check if edge was actually added.
			graph_.at(src).emplace(make_pair(dst, weight));
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

		// TODO(): implement
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

		// TODO(): implement
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
			res.sort();
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
			res.sort();
			return res;
		}

		// TODO(): implement
		// [[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator;

		[[nodiscard]] auto connections(N const& src) -> std::vector<N> {
			auto res = std::vector<N>{};
			for (auto const& conn : graph_.at(src)) {
				res.push_back(conn.first);
			}
			// TODO(): sort in asc order with respect to connected nodes
			res.sort();
			return res;
		}
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
