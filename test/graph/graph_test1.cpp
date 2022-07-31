/*
Testing Rationale
- Tests were created with the intent of discovering logic errors/bugs by verifying test output with
manually computed expected outputs (i.e. from the assignment spec).
- Correct outputs in tests were calculated manually and checked multiple times to ensure
correctness and to avoid bias towards implementation details.
- Checks were done using multiple internal functions to ensure test correctness even if one check
fails. This also makes it easier to check if a function is working properly.
- Proper code coverage was ensured by going through the code with test case inputs manually.
- All test cases were made to be readable and easily understood/self explanatory.
*/

#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

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

		auto const& [l_src, l_dst, l_weight] = *last;
		CHECK(l_src == 3);
		CHECK(l_dst == 1);
		CHECK(l_weight == 1);

		CHECK(g.is_connected(3, 1));
		CHECK(g.weights(3, 1).front() == 1);
		CHECK(g.find(3, 1, 1) != g.end());

		CHECK(g.erase_edge(last, g.end()) == g.end());
	}
}

TEST_CASE("Graph clearing (clear())") {
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

TEST_CASE("Node existence accessor (is_node())") {
	auto const& g = gdwg::graph<int, int>{1, 2, 3};
	CHECK(g.is_node(1));
	CHECK(g.is_node(2));
	CHECK(g.is_node(3));

	CHECK_FALSE(g.is_node(4));
	CHECK_FALSE(g.is_node(5));
	CHECK_FALSE(g.is_node(6));
}

TEST_CASE("Empty accessor (empty())") {
	auto const& g1 = gdwg::graph<int, int>();
	CHECK(g1.nodes().empty());
	CHECK(g1.empty());
}

TEST_CASE("Edge existence accessor (is_connected())") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(2, 3, 1));
	REQUIRE(g.insert_edge(3, 1, 1));

	SECTION("The existence of edges between two nodes can be checked") {
		CHECK(g.is_connected(1, 2));
		CHECK(g.is_connected(2, 3));
		CHECK(g.is_connected(3, 1));

		CHECK_FALSE(g.is_connected(2, 1));
		CHECK_FALSE(g.is_connected(3, 2));
		CHECK_FALSE(g.is_connected(1, 3));
	}

	SECTION("is_connected() throws if src or dst nodes do not exist in graph") {
		auto const& exception_msg = "Cannot call gdwg::graph<N, E>::is_connected if src or dst node "
		                            "don't exist in the graph";

		CHECK_THROWS_WITH(g.is_connected(1, 4), exception_msg);
		CHECK_THROWS_WITH(g.is_connected(4, 1), exception_msg);
		CHECK_THROWS_WITH(g.is_connected(4, 4), exception_msg);

		CHECK_THROWS_AS(g.is_connected(1, 4), std::runtime_error);
		CHECK_THROWS_AS(g.is_connected(4, 1), std::runtime_error);
		CHECK_THROWS_AS(g.is_connected(4, 4), std::runtime_error);
	}
}

TEST_CASE("Node accessor (nodes())") {
	auto const& g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE_FALSE(g.empty());

	auto const& nodes = g.nodes();
	CHECK(nodes.size() == 3);
	CHECK(nodes.front() == 1);
	CHECK(nodes.at(1) == 2);
	CHECK(nodes.back() == 3);
}

TEST_CASE("Edge weight accessor (weights())") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(1, 2, 2));
	REQUIRE(g.insert_edge(1, 2, 3));
	REQUIRE(g.insert_edge(2, 3, 4));
	REQUIRE(g.insert_edge(3, 1, 5));
	REQUIRE(g.insert_edge(3, 3, 5));
	REQUIRE(g.insert_edge(3, 3, 6));

	SECTION("Edge weights can be returned") {
		CHECK(g.weights(1, 2) == std::vector<int>{1, 2, 3});
		CHECK(g.weights(2, 3).front() == 4);
		CHECK(g.weights(3, 1).front() == 5);
		CHECK(g.weights(3, 3).front() == 5);
		CHECK(g.weights(3, 3).back() == 6);
	}

	SECTION("weights() throws if src or dst nodes do not exist in graph") {
		auto const& exception_msg = "Cannot call gdwg::graph<N, E>::weights if src or dst node don't "
		                            "exist in the graph";

		CHECK_THROWS_WITH(g.weights(1, 4), exception_msg);
		CHECK_THROWS_WITH(g.weights(4, 1), exception_msg);
		CHECK_THROWS_WITH(g.weights(4, 4), exception_msg);

		CHECK_THROWS_AS(g.weights(1, 4), std::runtime_error);
		CHECK_THROWS_AS(g.weights(4, 1), std::runtime_error);
		CHECK_THROWS_AS(g.weights(4, 4), std::runtime_error);
	}
}

TEST_CASE("Edge accessor (find())") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(2, 3, 1));
	REQUIRE(g.insert_edge(3, 1, 1));

	SECTION("Iterators to matching edges can be returned") {
		auto const& edge1_it = g.find(1, 2, 1);
		auto const& [src1, dst1, weight1] = *edge1_it;
		CHECK(src1 == 1);
		CHECK(dst1 == 2);
		CHECK(weight1 == 1);

		auto const& edge2_it = g.find(2, 3, 1);
		auto const& [src2, dst2, weight2] = *edge2_it;
		CHECK(src2 == 2);
		CHECK(dst2 == 3);
		CHECK(weight2 == 1);

		auto const& edge3_it = g.find(3, 1, 1);
		auto const& [src3, dst3, weight3] = *edge3_it;
		CHECK(src3 == 3);
		CHECK(dst3 == 1);
		CHECK(weight3 == 1);
	}

	SECTION("find() returns end() if a given edge does not exist") {
		// edge 1->2 exists but different weight
		CHECK(g.find(1, 2, 3) == g.end());

		// node 4 does not exist
		CHECK(g.find(3, 4, 1) == g.end());
		CHECK(g.find(4, 3, 1) == g.end());
	}
}

TEST_CASE("Node connections accessor (connections())") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(1, 2, 2));
	REQUIRE(g.insert_edge(1, 2, 3));
	REQUIRE(g.insert_edge(2, 3, 4));
	REQUIRE(g.insert_edge(3, 1, 5));
	REQUIRE(g.insert_edge(3, 3, 5));
	REQUIRE(g.insert_edge(3, 3, 6));

	SECTION("All connections from a given node can be returned") {
		// 3 of the same connections but with different weights
		CHECK(g.connections(1).front() == 2);

		// 1 connection
		CHECK(g.connections(2).front() == 3);

		// 2 of the same connections with different weights + 1 different connection
		CHECK(g.connections(3).front() == 1);
		CHECK(g.connections(3).back() == 3);
	}

	SECTION("connections() throws if src does not exist in graph") {
		auto const& exception_msg = "Cannot call gdwg::graph<N, E>::connections if src doesn't exist "
		                            "in the graph";

		CHECK_THROWS_WITH(g.connections(4), exception_msg);
		CHECK_THROWS_AS(g.connections(4), std::runtime_error);
	}
}

TEST_CASE("Begin iterator (begin())") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(3, 1, 1));

	auto it = g.begin();
	auto const& [src1, dst1, weight1] = *it;
	CHECK(src1 == 1);
	CHECK(dst1 == 2);
	CHECK(weight1 == 1);
	CHECK(g.find(1, 2, 1) == it);
}

TEST_CASE("End iterator (end())") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(3, 1, 1));

	auto it = g.begin();
	++it;
	++it;

	CHECK(it == g.end());
}

TEST_CASE("Equality testing (operator==)") {
	auto g1 = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g1.nodes().size() == 3);
	REQUIRE(g1.insert_edge(1, 2, 1));
	REQUIRE(g1.insert_edge(3, 1, 1));

	SECTION("Graphs with the same nodes and edges are equal") {
		auto g2 = gdwg::graph<int, int>{1, 2, 3};
		REQUIRE(g2.nodes().size() == 3);
		REQUIRE(g2.insert_edge(1, 2, 1));
		REQUIRE(g2.insert_edge(3, 1, 1));
		CHECK(g1 == g2);
		CHECK(g1 == g1);
		CHECK(g2 == g2);
	}

	SECTION("Graphs with the same nodes but different edges are not equal") {
		auto g2 = gdwg::graph<int, int>{1, 2, 3};
		REQUIRE(g2.nodes().size() == 3);
		REQUIRE(g2.insert_edge(1, 2, 1));
		REQUIRE(g2.insert_edge(2, 3, 1));
		CHECK(g1 != g2);

		// replace different edge
		REQUIRE(g2.erase_edge(2, 3, 1));
		REQUIRE(g2.insert_edge(3, 1, 1));
		CHECK(g1 == g2);

		// same edge, different weight
		REQUIRE(g2.insert_edge(3, 1, 12));
		CHECK(g1 != g2);
	}

	SECTION("Graphs with different nodes are not equal") {
		auto g2 = gdwg::graph<int, int>{4, 5, 6};
		REQUIRE(g2.nodes().size() == 3);
		CHECK(g1 != g2);
	}

	SECTION("Graphs with different edges but same weights are not equal") {
		auto g2 = gdwg::graph<int, int>{1, 2, 3};
		REQUIRE(g2.nodes().size() == 3);
		REQUIRE(g2.insert_edge(2, 1, 1));
		REQUIRE(g2.insert_edge(1, 3, 1));
		CHECK(g1 != g2);
	}

	SECTION("Empty graphs are equal") {
		auto const& g2 = gdwg::graph<int, int>{};
		REQUIRE(g2.nodes().empty());

		auto const& g3 = gdwg::graph<int, int>{};
		REQUIRE(g3.nodes().empty());
		CHECK(g2 == g3);
	}
}

TEST_CASE("Graph extractor (operator<<)") {
	// test case taken from assignment spec
	SECTION("Assignment spec example 1") {
		auto g = gdwg::graph<int, int>{};
		g.insert_node(4);
		g.insert_node(1);
		g.insert_edge(4, 1, -4);
		g.insert_node(3);
		g.insert_node(2);
		g.insert_edge(3, 2, 2);
		g.insert_node(2);
		g.insert_node(4);
		g.insert_edge(2, 4, 2);
		g.insert_node(2);
		g.insert_node(1);
		g.insert_edge(2, 1, 1);
		g.insert_node(6);
		g.insert_node(2);
		g.insert_edge(6, 2, 5);
		g.insert_node(6);
		g.insert_node(3);
		g.insert_edge(6, 3, 10);
		g.insert_node(1);
		g.insert_node(5);
		g.insert_edge(1, 5, -1);
		g.insert_node(3);
		g.insert_node(6);
		g.insert_edge(3, 6, -8);
		g.insert_node(4);
		g.insert_node(5);
		g.insert_edge(4, 5, 3);
		g.insert_node(5);
		g.insert_node(2);
		g.insert_edge(5, 2, 7);
		g.insert_node(64);

		auto out = std::ostringstream{};
		out << g;

		auto expected_output = std::string{};
		expected_output.append("1 (\n");
		expected_output.append("  5 | -1\n");
		expected_output.append(")\n");
		expected_output.append("2 (\n");
		expected_output.append("  1 | 1\n");
		expected_output.append("  4 | 2\n");
		expected_output.append(")\n");
		expected_output.append("3 (\n");
		expected_output.append("  2 | 2\n");
		expected_output.append("  6 | -8\n");
		expected_output.append(")\n");
		expected_output.append("4 (\n");
		expected_output.append("  1 | -4\n");
		expected_output.append("  5 | 3\n");
		expected_output.append(")\n");
		expected_output.append("5 (\n");
		expected_output.append("  2 | 7\n");
		expected_output.append(")\n");
		expected_output.append("6 (\n");
		expected_output.append("  2 | 5\n");
		expected_output.append("  3 | 10\n");
		expected_output.append(")\n");
		expected_output.append("64 (\n");
		expected_output.append(")\n");

		CHECK(out.str() == expected_output);
	}

	SECTION("Extractor works with given test case") {
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("hello");
		g.insert_node("how");
		g.insert_node("are");
		g.insert_node("you?");

		g.insert_edge("hello", "how", 5);
		g.insert_edge("hello", "are", 8);
		g.insert_edge("hello", "are", 2);

		g.insert_edge("how", "you?", 1);
		g.insert_edge("how", "hello", 4);

		g.insert_edge("are", "you?", 3);
		auto out = std::ostringstream{};
		out << g;

		auto expected_output = std::string{};
		expected_output.append("are (\n");
		expected_output.append("  you? | 3\n");
		expected_output.append(")\n");
		expected_output.append("hello (\n");
		expected_output.append("  are | 2\n");
		expected_output.append("  are | 8\n");
		expected_output.append("  how | 5\n");
		expected_output.append(")\n");
		expected_output.append("how (\n");
		expected_output.append("  hello | 4\n");
		expected_output.append("  you? | 1\n");
		expected_output.append(")\n");
		expected_output.append("you? (\n");
		expected_output.append(")\n");
		CHECK(out.str() == expected_output);
	}

	SECTION("Extractor works for empty graphs") {
		auto const& g = gdwg::graph<int, int>();
		auto oss = std::ostringstream{};
		oss << g;
		CHECK(oss.str().empty());
	}
}

TEST_CASE("Iterator operators") {
	auto g = gdwg::graph<int, int>{1, 2, 3};
	REQUIRE(g.nodes().size() == 3);
	REQUIRE(g.insert_edge(1, 2, 1));
	REQUIRE(g.insert_edge(3, 1, 1));

	SECTION("Prefix increment operator correctly moves iterator forward") {
		auto it = g.begin();
		CHECK(g.find(1, 2, 1) == it);

		++it;
		CHECK(g.find(3, 1, 1) == it);

		++it;
		CHECK(it == g.end());
	}

	SECTION("Postfix increment operator correctly returns") {
		auto it1 = g.begin();
		CHECK(it1++ == g.begin());
	}

	SECTION("Prefix decrement operator correctly moves iterator backwards") {
		auto it = g.end();
		--it;

		CHECK(it == g.find(3, 1, 1));

		--it;
		CHECK(g.find(1, 2, 1) == it);

		CHECK(it == g.begin());
	}

	SECTION("Postfix decrement operator correctly returns") {
		auto it1 = g.end();
		CHECK(it1-- == g.end());
	}

	SECTION("Iterator equality can be checked") {
		auto it1 = g.begin();
		auto it2 = g.begin();
		CHECK(it1 == it2);

		++it1;
		++it2;
		CHECK(it1 == it2);

		++it1;
		++it2;
		CHECK(it1 == it2);

		CHECK(it1 == g.end());
		CHECK(it2 == g.end());

		--it1;
		CHECK(it1 == g.find(3, 1, 1));

		--it2;
		--it2;
		CHECK(it2 == g.find(1, 2, 1));
	}

	SECTION("Iterators can be dereferenced") {
		auto it = g.begin();
		auto const& [src1, dst1, weight1] = *it;
		CHECK(src1 == 1);
		CHECK(dst1 == 2);
		CHECK(weight1 == 1);
		CHECK(g.find(1, 2, 1) == it);

		++it;
		auto const& [src2, dst2, weight2] = *it;
		CHECK(src2 == 3);
		CHECK(dst2 == 1);
		CHECK(weight2 == 1);
		CHECK(g.find(3, 1, 1) == it);

		--it;
		auto const& [src3, dst3, weight3] = *it;
		CHECK(src3 == 1);
		CHECK(dst3 == 2);
		CHECK(weight3 == 1);
		CHECK(g.find(1, 2, 1) == it);
	}
}