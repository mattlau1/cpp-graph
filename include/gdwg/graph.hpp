#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <unordered_map>
#include <vector>

// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		graph() noexcept
		: g_{std::unordered_map<N, std::vector<E>>{}} {};

		graph(std::initializer_list<N> il)
		: g_{std::unordered_map<N, std::vector<E>>{}} {
			for (auto const i : il) {
				g_.emplace(i, std::vector<E>{});
			}
		}

		template<typename InputIt>
		graph(InputIt first, InputIt last)
		: g_{std::unordered_map<N, std::vector<E>>{}} {
			for (auto f = first; f != last; ++first) {
				g_.emplace(*f, std::vector<E>{});
			}
		}

		graph(graph&& other) noexcept;
		auto operator=(graph&& other) noexcept -> graph&;
		graph(graph const& other);
		auto operator=(graph const& other) -> graph&;

	private:
		std::unordered_map<N, std::vector<E>> g_;
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
