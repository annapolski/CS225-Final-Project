// TESTS for BFS

#include <catch2/catch_test_macros.hpp>
#include "graph.h"
#include "utilities.h"
#include <iostream>
#include <set>

TEST_CASE("Data parsing from CSV") {
    Graph g("../bfs_test_data.csv");
    

    SECTION("target 10%, range 0") {
        std::vector<Graph::Node*> result = g.findByLoss(.1, 0);
        REQUIRE(result.size() == 1);
        REQUIRE(result.at(0)->incidentID == "a");

    }

    SECTION("target 10%, range 0.05") {
        std::vector<Graph::Node*> result = g.findByLoss(.1, 0.05);
        std::set<std::string> expected = {"a", "b", "c", "d"};
        REQUIRE(result.size() == 4);
        for (auto n : result) {
            REQUIRE(expected.find(n->incidentID) != expected.end());
            expected.erase(n->incidentID);
        }
    }
    
    SECTION("target 15%, range 0") {
       std::vector<Graph::Node*> result = g.findByLoss(0.15, 0);
       REQUIRE(result.size() == 2);
       REQUIRE(result.at(0)->incidentID == "b" || result.at(0)->incidentID == "c");
       REQUIRE(result.at(1)->incidentID == "c" || result.at(1)->incidentID == "b");
    }

    SECTION("target 15%, range 0.05") {
        std::vector<Graph::Node*> result = g.findByLoss(0.15, 0.05);
        std::set<std::string> expected = {"a", "b", "c"};
        REQUIRE(result.size() == 3);
        for (auto n : result) {
            REQUIRE(expected.find(n->incidentID) != expected.end());
            expected.erase(n->incidentID);
        }
    }

    SECTION("target 50%, range 0.25") {
        std::vector<Graph::Node*> result = g.findByLoss(0.5, 0.25);
        std::set<std::string> expected = {"e", "h", "i"};
        REQUIRE(result.size() == 3);
        for (auto n : result) {
            REQUIRE(expected.find(n->incidentID) != expected.end());
            expected.erase(n->incidentID);
        }
    }
}
