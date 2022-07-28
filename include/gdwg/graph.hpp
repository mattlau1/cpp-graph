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
			// return graph_.emplace(value, edge{})->second;
			if (!graph_.contains(value)) {
				return false;
			}
			graph_.emplace(value, edge{});
			return true;
		};

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!graph_.contains(src) || !graph_.contains(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
				return false;
			}
			// TODO(): check if edge was actually added.
			graph_.at(src).emplace(make_pair(dst, weight));
			return true;
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!graph_.contains(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
				return false;
			}
			if (!graph_.contains(new_data)) {
				return false;
			}

			for (auto const& conn : graph_) {
				if (conn->first != old_data) {
					// replace reference to old_data in conn with new_data
					for (auto const& e : graph_.at(conn)) {
						if (e->first == old_data) {
							e->first = new_data;
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
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
