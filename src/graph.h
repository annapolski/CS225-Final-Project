#pragma once

#include <cmath>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <map>

using std::pair;
using std::priority_queue;
using std::string;
using std::set;
using std::vector;
using std::map;

class Graph {
    public: 
    /**
     * Node struct to represent each incident on the graph
    */
    struct Node {
        string incidentID;
        int totalLoss;
        int totalMigrants;
        pair<double, double> coordinates; 
    };

    /**
     * Default constructor
    */
    Graph() = default;

    /**
     * Parametrized constructor that takes in a .csv file where each column
     * represents incidentID, totDeadMissing, totalMigrants, xCoord, and yCoord
     * 
     * @param filename .csv file of incidents
    */
    Graph(string filename);
    
    /**
     * Returns shortest path from root of spanning tree to @param node
     * Dijkstras
    */
    vector<Node*> getShortestPath(Node* node);
    

    private:
    vector<vector<string>> data_;

    map<Node*, vector<pair<Node*, double>>> edgeList_;

    // shortest paths
    map<Node*, vector<Node*>> short_paths_;

    // set of all vertices in the Graph
    set<Node*> vertices_;

    /*
    Node with the lowest fraction of lost migrants.
    Initialized in the Graph constructor.
    */
    Node* lowest_risk_;

    /**
     * Traverses spanning tree to get the Shortest Path
    */
    vector<Node*> getShortestPath(Node* node);

    /**
     * Finding the distance between two coordinates in radians using the haversine formula @endparblock
     * 
     * degrees to radians: degrees ⋅ pi / 180
     * 
     * φ = latitude
     * λ = longitude
     * R = Earth's radius = 6,371km
     * 
     * Haversine Formula:
     * a = sin²(Δφ/2) + cos(φ1) ⋅ cos(φ2) ⋅ sin²(Δλ/2)
     * c = 2 ⋅ atan2( √a, √(1−a) )
     * distance = R ⋅ c
     * 
     * @cite https://www.movable-type.co.uk/scripts/latlong.html
    */
    double calculateDistance(Node* one, Node* two);

    /*
    Returns the fraction of lost migrants out of the total number.
    */
    double calculateRisk(Node* node);

    void dijkstra(Node* node);
};