#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <memory>
#include <stdexcept>

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
		CHECK(g.insert_node(5));
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
		CHECK(g.insert_node(5));
		CHECK(g.is_node(5));
	}

	SECTION("Graph can be constructed with a non empty initialiser list") {
		auto const& g2 = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
		CHECK_FALSE(g2.empty());
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
	CHECK_FALSE(g1.empty());
	CHECK(g1.nodes().size() == 3);
	CHECK(g1.nodes() == std::vector<int>{1, 2, 3});
	CHECK(g1.is_node(1));
	CHECK(g1.is_node(2));
	CHECK(g1.is_node(3));

	auto const& node_set = std::set<int>{1, 2, 3};
	auto const& g2 = gdwg::graph<int, int>(node_set.begin(), node_set.end());
	CHECK_FALSE(g2.empty());
	CHECK(g2.nodes().size() == 3);
	CHECK(g2.nodes() == std::vector<int>{1, 2, 3});
	CHECK(g2.is_node(1));
	CHECK(g2.is_node(2));
	CHECK(g2.is_node(3));
}

TEST_CASE("Move constructor") {
	auto g1 = gdwg::graph<int, int>{1, 2};
	REQUIRE(g1.insert_edge(1, 2, 1));

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
	CHECK_FALSE(g1.is_node(src));
	CHECK_FALSE(g1.is_node(dst));

	CHECK(g2.is_node(src));
	CHECK(g2.is_node(dst));
	CHECK(g2.is_connected(src, dst));
}

TEST_CASE("Move assignment") {
	auto g1 = gdwg::graph<int, int>{1, 2};
	REQUIRE(g1.insert_edge(1, 2, 1));
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
	CHECK_FALSE(g1.is_node(src));
	CHECK_FALSE(g1.is_node(dst));

	CHECK(g2.is_node(src));
	CHECK(g2.is_node(dst));
	CHECK(g2.is_connected(src, dst));
}

TEST_CASE("Copy constructor") {
	auto g1 = gdwg::graph<int, int>{1, 2};
	REQUIRE(g1.insert_edge(1, 2, 1));
	REQUIRE(g1.insert_edge(2, 1, 100));
	auto const& g2 = gdwg::graph<int, int>(g1);

	// g1 should have valid nodes and edges
	REQUIRE_FALSE(g1.empty());
	REQUIRE(g1.is_node(1));
	REQUIRE(g1.is_node(2));
	REQUIRE(g1.is_connected(1, 2));
	REQUIRE(g1.is_connected(2, 1));

	// g2 should have the same nodes and edges
	CHECK_FALSE(g2.empty());
	CHECK(g2.is_node(1));
	CHECK(g2.is_node(2));
	CHECK(g2.is_connected(1, 2));
	CHECK(g2.is_connected(2, 1));
	CHECK(g2 == g1);
}

TEST_CASE("Copy assignment") {
	auto g1 = gdwg::graph<int, int>{1, 2};
	REQUIRE(g1.insert_edge(1, 2, 1));
	REQUIRE(g1.insert_edge(2, 1, 100));
	auto const& g2 = g1;

	// g1 should have valid nodes and edges
	REQUIRE_FALSE(g1.empty());
	REQUIRE(g1.is_node(1));
	REQUIRE(g1.is_node(2));
	REQUIRE(g1.is_connected(1, 2));
	REQUIRE(g1.is_connected(2, 1));

	// g2 should have the same nodes and edges
	CHECK_FALSE(g2.empty());
	CHECK(g2.is_node(1));
	CHECK(g2.is_node(2));
	CHECK(g2.is_connected(1, 2));
	CHECK(g2.is_connected(2, 1));
	CHECK(g2 == g1);
}

TEST_CASE("Node insertion (insert_node())") {
	auto g = gdwg::graph<std::string, int>{};
	REQUIRE(g.empty());

	// successful node insertion
	CHECK(g.insert_node("1"));
	CHECK(g.insert_node("2"));

	// unsuccessful insertion due to duplicate
	CHECK_FALSE(g.insert_node("2"));

	CHECK_FALSE(g.empty());
	CHECK(g.nodes().size() == 2);
	CHECK(g.nodes() == std::vector<std::string>{"1", "2"});
}

TEST_CASE("Edge insertion (insert_edge())") {
	auto g = gdwg::graph<int, std::string>{};
	REQUIRE(g.insert_node(1));
	REQUIRE(g.insert_node(2));
	REQUIRE(g.insert_node(3));
	REQUIRE_FALSE(g.empty());
	REQUIRE(g.nodes() == std::vector<int>{1, 2, 3});

	SECTION("Edges can be validly inserted") {
		CHECK(g.insert_edge(1, 2, "a"));
		CHECK(g.is_connected(1, 2));
		CHECK(g.weights(1, 2).front() == "a");
		CHECK(g.find(1, 2, "a") != g.end());

		CHECK(g.insert_edge(2, 2, "b"));
		CHECK(g.is_connected(2, 2));
		CHECK(g.weights(2, 2).front() == "b");
		CHECK(g.find(2, 2, "b") != g.end());

		CHECK(g.insert_edge(2, 1, "c"));
		CHECK(g.is_connected(2, 1));
		CHECK(g.weights(2, 1).front() == "c");
		CHECK(g.find(2, 1, "c") != g.end());
	}

	SECTION("Duplicate edges cannot be added") {
		CHECK(g.insert_edge(1, 3, "a"));
		CHECK_FALSE(g.insert_edge(1, 3, "a"));
		CHECK(g.is_connected(1, 3));
		CHECK(g.connections(1).size() == 1);
		CHECK(g.connections(1).front() == 3);
		CHECK(g.weights(1, 3).size() == 1);
		CHECK(g.weights(1, 3).front() == "a");
		CHECK(g.find(1, 3, "a") != g.end());
	}

	SECTION("insert_edge() throws if src or dst nodes do not exist in graph") {
		auto const& exception_msg = "Cannot call gdwg::graph<N, E>::insert_edge when either src or "
		                            "dst node does not "
		                            "exist";
		// node 4 does not exist in graph g
		CHECK_THROWS_WITH(g.insert_edge(4, 1, "a"), exception_msg);
		CHECK_THROWS_WITH(g.insert_edge(1, 4, "a"), exception_msg);
		CHECK_THROWS_WITH(g.insert_edge(4, 4, "a"), exception_msg);

		CHECK_THROWS_AS(g.insert_edge(4, 1, "a"), std::runtime_error);
		CHECK_THROWS_AS(g.insert_edge(1, 4, "a"), std::runtime_error);
		CHECK_THROWS_AS(g.insert_edge(4, 4, "a"), std::runtime_error);
	}
}

TEST_CASE("Node replacement (replace_node())") {
	auto g = gdwg::graph<std::string, int>{"a", "b", "c"};
	REQUIRE(g.nodes().size() == 3);

	SECTION("Nodes can be replaced") {
		CHECK(g.replace_node("a", "d"));
		CHECK(g.replace_node("b", "e"));
		CHECK(g.replace_node("c", "f"));
		CHECK(g.nodes() == std::vector<std::string>{"d", "e", "f"});
	}

	SECTION("Nodes with edges can be correctly replaced") {
		REQUIRE(g.insert_edge("a", "b", 1));
		REQUIRE(g.insert_edge("b", "c", 2));
		REQUIRE(g.insert_edge("c", "a", 3));
		REQUIRE(g.insert_edge("a", "c", 4));

		REQUIRE(g.is_connected("b", "c"));
		REQUIRE(g.is_connected("c", "a"));
		REQUIRE(g.is_connected("a", "c"));

		CHECK(g.replace_node("c", "d"));

		CHECK_FALSE(g.is_node("c"));
		CHECK(g.is_node("d"));

		// all edges to/from "c" should now connect to "d" instead
		CHECK(g.is_connected("b", "d"));
		CHECK(g.is_connected("d", "a"));
		CHECK(g.is_connected("a", "d"));

		CHECK(g.nodes() == std::vector<std::string>{"a", "b", "d"});
		CHECK(g.connections("d") == std::vector<std::string>{"a"});
	}

	SECTION("replace_node() returns false and nothing happens if new_data is already a node") {
		CHECK_FALSE(g.replace_node("a", "b"));
		CHECK_FALSE(g.replace_node("b", "b"));

		CHECK(g.nodes() == std::vector<std::string>{"a", "b", "c"});
	}

	SECTION("replace_node() throws if old_data is not a node") {
		auto const& exception_msg = "Cannot call gdwg::graph<N, E>::replace_node on a node that "
		                            "doesn't exist";
		CHECK_THROWS_WITH(g.replace_node("d", "abc"), exception_msg);
		CHECK_THROWS_AS(g.replace_node("d", "abc"), std::runtime_error);
	}
}

TEST_CASE("Node merge replacement (merge_replace_node())") {
	auto g = gdwg::graph<std::string, int>{"a", "b", "c", "d"};
	REQUIRE(g.nodes().size() == 4);

	SECTION("Nodes can be merge replaced") {
		SECTION("Assignment spec example 1") {
			REQUIRE(g.insert_edge("a", "b", 1));
			REQUIRE(g.insert_edge("a", "c", 2));
			REQUIRE(g.insert_edge("a", "d", 3));

			CHECK_NOTHROW(g.merge_replace_node("a", "b"));

			CHECK_FALSE(g.is_node("a"));

			CHECK(g.is_connected("b", "b"));
			CHECK(g.weights("b", "b").size() == 1);
			CHECK(g.weights("b", "b").front() == 1);

			CHECK(g.is_connected("b", "c"));
			CHECK(g.weights("b", "c").size() == 1);
			CHECK(g.weights("b", "c").front() == 2);

			CHECK(g.is_connected("b", "d"));
			CHECK(g.weights("b", "d").size() == 1);
			CHECK(g.weights("b", "d").front() == 3);
		}

		SECTION("Assignment spec example 2") {
			// duplicate edge should be removed
			REQUIRE(g.insert_edge("a", "b", 1));
			REQUIRE(g.insert_edge("a", "c", 2));
			REQUIRE(g.insert_edge("a", "d", 3));
			REQUIRE(g.insert_edge("b", "b", 1));

			CHECK_NOTHROW(g.merge_replace_node("a", "b"));

			CHECK_FALSE(g.is_node("a"));

			CHECK(g.is_connected("b", "b"));
			CHECK(g.weights("b", "b").size() == 1);
			CHECK(g.weights("b", "b").front() == 1);

			CHECK(g.is_connected("b", "c"));
			CHECK(g.weights("b", "c").size() == 1);
			CHECK(g.weights("b", "c").front() == 2);

			CHECK(g.is_connected("b", "d"));
			CHECK(g.weights("b", "d").size() == 1);
			CHECK(g.weights("b", "d").front() == 3);
		}

		SECTION("Assignment spec example 3") {
			REQUIRE(g.insert_edge("a", "b", 3));
			REQUIRE(g.insert_edge("c", "b", 2));
			REQUIRE(g.insert_edge("d", "b", 4));

			CHECK_NOTHROW(g.merge_replace_node("b", "a"));

			CHECK_FALSE(g.is_node("b"));

			CHECK(g.is_connected("a", "a"));
			CHECK(g.weights("a", "a").size() == 1);
			CHECK(g.weights("a", "a").front() == 3);

			CHECK(g.is_connected("c", "a"));
			CHECK(g.weights("c", "a").size() == 1);
			CHECK(g.weights("c", "a").front() == 2);

			CHECK(g.is_connected("d", "a"));
			CHECK(g.weights("d", "a").size() == 1);
			CHECK(g.weights("d", "a").front() == 4);
		}

		SECTION("Assignment spec example 4") {
			REQUIRE(g.insert_edge("b", "a", 3));
			REQUIRE(g.insert_edge("b", "c", 2));
			REQUIRE(g.insert_edge("b", "d", 4));

			CHECK_NOTHROW(g.merge_replace_node("b", "a"));

			CHECK_FALSE(g.is_node("b"));

			CHECK(g.is_connected("a", "a"));
			CHECK(g.weights("a", "a").size() == 1);
			CHECK(g.weights("a", "a").front() == 3);

			CHECK(g.is_connected("a", "c"));
			CHECK(g.weights("a", "c").size() == 1);
			CHECK(g.weights("a", "c").front() == 2);

			CHECK(g.is_connected("a", "d"));
			CHECK(g.weights("a", "d").size() == 1);
			CHECK(g.weights("a", "d").front() == 4);
		}
	}

	SECTION("merge_replace_node() throws if old_data or new_data is not a node") {
		auto const& exception_msg = "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new "
		                            "data if they don't exist in the graph";

		CHECK_THROWS_WITH(g.merge_replace_node("abc", "a"), exception_msg);
		CHECK_THROWS_AS(g.merge_replace_node("abc", "a"), std::runtime_error);

		CHECK_THROWS_WITH(g.merge_replace_node("a", "abc"), exception_msg);
		CHECK_THROWS_AS(g.merge_replace_node("a", "abc"), std::runtime_error);

		CHECK_THROWS_WITH(g.merge_replace_node("abc", "def"), exception_msg);
		CHECK_THROWS_AS(g.merge_replace_node("abc", "def"), std::runtime_error);
	}
}

TEST_CASE("Node erasure (erase_node())") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);

	SECTION("Nodes can be erased from graphs without edges") {
		CHECK(g.erase_node(2));
		CHECK_FALSE(g.is_node(2));
		CHECK(g.nodes() == std::vector<int>{1, 3});

		CHECK(g.erase_node(3));
		CHECK_FALSE(g.is_node(3));
		CHECK(g.nodes() == std::vector<int>{1});

		CHECK(g.erase_node(1));
		CHECK_FALSE(g.is_node(1));
		CHECK(g.nodes().empty());
	}

	SECTION("Nodes can be erased from graphs with edges") {
		REQUIRE(g.insert_edge(1, 3, 1));
		REQUIRE(g.insert_edge(3, 1, 1));
		REQUIRE(g.insert_edge(1, 2, 1));
		REQUIRE(g.insert_edge(3, 2, 1));

		CHECK(g.erase_node(2));
		CHECK_FALSE(g.is_node(2));
		CHECK(g.nodes() == std::vector<int>{1, 3});

		// edges 1---1-->2 & 3---1-->2 should not exist
		CHECK_THROWS(g.is_connected(1, 2));
		CHECK_THROWS(g.is_connected(3, 2));

		CHECK(g.find(1, 2, 1) == g.end());
		CHECK(g.find(3, 2, 1) == g.end());

		// check if node 2 was erased from edges
		CHECK(g.connections(1) == std::vector{3});
		CHECK(g.connections(3) == std::vector{1});
	}

	SECTION("erase_node() returns false if node was not erased") {
		REQUIRE_FALSE(g.is_node(4));
		CHECK_FALSE(g.erase_node(4));
	}
}

TEST_CASE("Edge erasure (erase_edge(src, dst, weight))") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(2, 3, 1));
	REQUIRE(g.insert_edge(3, 1, 1));

	SECTION("Edges can be erased given a source, destination and weight") {
		CHECK(g.is_connected(1, 2));
		CHECK(g.find(1, 2, 1) != g.end());

		CHECK(g.erase_edge(1, 2, 1));

		CHECK_FALSE(g.is_connected(1, 2));
		CHECK(g.find(1, 2, 1) == g.end());
	}

	SECTION("Edges cannot be erased if there is no matching edge between src and dst") {
		// edge 2->1 does not exist
		CHECK_FALSE(g.erase_edge(2, 1, 1));

		// edge 1->2 exists but with weight 1
		CHECK_FALSE(g.erase_edge(1, 2, 12));
	}

	SECTION("erase_edge(src, dst, weight) throws if src or dst do not exist") {
		auto const& exception_msg = "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
		                            "they don't exist in the graph";

		CHECK_THROWS_WITH(g.erase_edge(1, 4, 14), exception_msg);
		CHECK_THROWS_AS(g.erase_edge(1, 4, 14), std::runtime_error);

		CHECK_THROWS_WITH(g.erase_edge(4, 1, 41), exception_msg);
		CHECK_THROWS_AS(g.erase_edge(4, 1, 41), std::runtime_error);

		CHECK_THROWS_WITH(g.erase_edge(69, 69, 69), exception_msg);
		CHECK_THROWS_AS(g.erase_edge(69, 69, 69), std::runtime_error);
	}
}

TEST_CASE("Edge erasure (erase_edge(iterator))") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(2, 3, 1));
	REQUIRE(g.insert_edge(3, 1, 1));
	SECTION("Edges can be erased given an iterator to the edge") {
		auto const& it = g.begin();
		auto const& [src, dst, weight] = *it;
		REQUIRE(src == 1);
		REQUIRE(dst == 2);
		REQUIRE(weight == 1);

		g.erase_edge(it);

		CHECK_FALSE(g.is_connected(1, 2));
		CHECK(g.find(1, 2, 1) == g.end());
	}

	SECTION("erase_edge(iterator) returns an iterator to the next edge") {
		auto const& begin_iter = g.begin();
		auto const& [src1, dst1, weight1] = *begin_iter;
		REQUIRE(src1 == 1);
		REQUIRE(dst1 == 2);
		REQUIRE(weight1 == 1);

		auto const& second_edge = g.erase_edge(begin_iter);
		auto const& [src2, dst2, weight2] = *second_edge;
		CHECK(src2 == 2);
		CHECK(dst2 == 3);
		CHECK(weight2 == 1);

		auto const& third_edge = g.erase_edge(second_edge);
		auto const& [src3, dst3, weight3] = *third_edge;
		CHECK(src3 == 3);
		CHECK(dst3 == 1);
		CHECK(weight3 == 1);

		// should return iterator to g.end(), since there are no more elements
		auto const& end = g.erase_edge(third_edge);
		CHECK(end == g.end());
	}
}

TEST_CASE("Edge erasure (erase_edge(iterator1, iterator2))") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(2, 3, 1));
	REQUIRE(g.insert_edge(3, 1, 1));

	SECTION("Edges can be erased given two iterators") {
		CHECK(g.erase_edge(g.begin(), g.end()) == g.end());
		CHECK_FALSE(g.is_connected(1, 2));
		CHECK_FALSE(g.is_connected(2, 3));
		CHECK_FALSE(g.is_connected(3, 1));
	}

	SECTION("erase_edge(iterator) returns an iterator to the next edge") {
		auto second_last = g.begin();
		++second_last;
		++second_last;
		auto const& last = g.erase_edge(g.begin(), second_last);
		CHECK(last != g.end());

		auto const& [src, dst, weight] = *last;
		CHECK(src == 3);
		CHECK(dst == 1);
		CHECK(weight == 1);

		CHECK(g.is_connected(3, 1));
		CHECK(g.find(3, 1, 1) != g.end());

		CHECK(g.erase_edge(last, g.end()) == g.end());
	}
}

TEST_CASE("Graph clearing (graph.clear())") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(2, 3, 1));
	REQUIRE(g.insert_edge(3, 1, 1));

	g.clear();
	CHECK_THROWS(g.is_connected(1, 2));
	CHECK_THROWS(g.is_connected(2, 3));
	CHECK_THROWS(g.is_connected(3, 1));

	CHECK(g.nodes().empty());
	CHECK(g.empty());
}
