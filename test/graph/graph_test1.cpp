#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("basic test") {
	// This will not compile straight away
	auto g = gdwg::graph<int, std::string>{};
	auto n = 5;
	g.insert_node(n);
	CHECK(g.is_node(n));
}

TEST_CASE("Default constructor") {
	SECTION("Graph is value initialised and empty") {
		auto const& g = gdwg::graph<int, std::string>();
		CHECK(g.empty());
		CHECK(g.nodes().empty());
		CHECK(g.begin() == g.end());
	}

	SECTION("Graph functions normally") {
		auto g = gdwg::graph<int, std::string>();
		g.insert_node(5);
		CHECK(g.is_node(5));
	}
}

TEST_CASE("Initialiser list constructor") {
	SECTION("Graph is empty") {
		auto const& g = gdwg::graph<int, std::string>{};
		CHECK(g.empty());
		CHECK(g.nodes().empty());
		CHECK(g.begin() == g.end());
	}

	SECTION("Graph functions normally") {
		auto g = gdwg::graph<int, std::string>{};
		g.insert_node(5);
		CHECK(g.is_node(5));
	}

	SECTION("Graph can be constructed with a non empty initialiser list") {
		auto const& g2 = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
		CHECK(!g2.empty());
		CHECK(g2.nodes().size() == 5);
		CHECK(g2.nodes() == std::vector<int>{1, 2, 3, 4, 5});
		CHECK(g2.is_node(1));
		CHECK(g2.is_node(2));
		CHECK(g2.is_node(3));
		CHECK(g2.is_node(4));
		CHECK(g2.is_node(5));
	}
}

TEST_CASE("Range constructor") {
	auto const& node_vec = std::vector<int>{1, 2, 3};
	auto const& g1 = gdwg::graph<int, int>(node_vec.begin(), node_vec.end());
	CHECK(!g1.empty());
	CHECK(g1.nodes().size() == 3);
	CHECK(g1.nodes() == std::vector<int>{1, 2, 3});
	CHECK(g1.is_node(1));
	CHECK(g1.is_node(2));
	CHECK(g1.is_node(3));

	auto const& node_set = std::set<int>{1, 2, 3};
	auto const& g2 = gdwg::graph<int, int>(node_set.begin(), node_set.end());
	CHECK(!g2.empty());
	CHECK(g2.nodes().size() == 3);
	CHECK(g2.nodes() == std::vector<int>{1, 2, 3});
	CHECK(g2.is_node(1));
	CHECK(g2.is_node(2));
	CHECK(g2.is_node(3));
}

TEST_CASE("Move constructor") {
	auto g1 = gdwg::graph<int, int>{1, 2};
	g1.insert_edge(1, 2, 1);

	// get iterator and copy of graph before move constructor is invoked
	auto const& g1_iter = g1.begin();
	auto g1_copy = g1;

	auto g2 = gdwg::graph<int, int>(std::move(g1));

	// *this (g2) is equal to the value other had before this constructor’s invocation.
	CHECK(g2 == g1_copy);

	// NOLINTNEXTLINE(bugprone-use-after-move)
	CHECK(g1.empty());

	// All iterators pointing to elements owned by other prior to this constructor’s invocation
	// remain valid, but now point to the elements owned by *this.
	auto const& [src, dst, _] = *g1_iter;
	CHECK(!g1.is_node(src));
	CHECK(!g1.is_node(dst));

	CHECK(g2.is_node(src));
	CHECK(g2.is_node(dst));
	CHECK(g2.is_connected(src, dst));
}

TEST_CASE("Move assignment") {
	auto g1 = gdwg::graph<int, int>{1, 2};
	g1.insert_edge(1, 2, 1);
	auto g1_copy = g1;
	auto const& g1_iter = g1.begin();

	auto g2 = std::move(g1);

	// *this (g2) is equal to the value other had before this constructor’s invocation.
	CHECK(g2 == g1_copy);

	// NOLINTNEXTLINE(bugprone-use-after-move)
	CHECK(g1.empty());

	// All iterators pointing to elements owned by other prior to this operator’s invocation remain
	// valid, but now point to the elements owned by *this.
	auto const& [src, dst, _] = *g1_iter;
	CHECK(!g1.is_node(src));
	CHECK(!g1.is_node(dst));

	CHECK(g2.is_node(src));
	CHECK(g2.is_node(dst));
	CHECK(g2.is_connected(src, dst));
}

TEST_CASE("Copy constructor") {
	auto g1 = gdwg::graph<int, int>{1, 2};
	g1.insert_edge(1, 2, 1);
	g1.insert_edge(2, 1, 100);
	auto const& g2 = gdwg::graph<int, int>(g1);

	// g1 should have valid nodes and edges
	REQUIRE(!g1.empty());
	REQUIRE(g1.is_node(1));
	REQUIRE(g1.is_node(2));
	REQUIRE(g1.is_connected(1, 2));
	REQUIRE(g1.is_connected(2, 1));

	// g2 should have the same nodes and edges
	CHECK(!g2.empty());
	CHECK(g2.is_node(1));
	CHECK(g2.is_node(2));
	CHECK(g2.is_connected(1, 2));
	CHECK(g2.is_connected(2, 1));
	CHECK(g2 == g1);
}

TEST_CASE("Copy assignment") {
	auto g1 = gdwg::graph<int, int>{1, 2};
	g1.insert_edge(1, 2, 1);
	g1.insert_edge(2, 1, 100);
	auto const& g2 = g1;

	// g1 should have valid nodes and edges
	REQUIRE(!g1.empty());
	REQUIRE(g1.is_node(1));
	REQUIRE(g1.is_node(2));
	REQUIRE(g1.is_connected(1, 2));
	REQUIRE(g1.is_connected(2, 1));

	// g2 should have the same nodes and edges
	CHECK(!g2.empty());
	CHECK(g2.is_node(1));
	CHECK(g2.is_node(2));
	CHECK(g2.is_connected(1, 2));
	CHECK(g2.is_connected(2, 1));
	CHECK(g2 == g1);
}