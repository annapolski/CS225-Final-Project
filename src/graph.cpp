#include "graph.h"
#include "utilities.h"

using std::atan2;
using std::cos;
using std::pair;
using std::pow;
using std::sin;
using std::sqrt;
using std::string;
using std::stod;
using std::stoi;
using std::set;
using std::vector;
using std::ifstream;

Graph::~Graph() {
    for (auto v: vertices_) {
        delete v;
    }
}

Graph::Graph(const Graph& other) {
    // @todo
}
    
Graph& Graph::operator=(const Graph& other) {
    // @todo
    if (this == &other) return *this;
    return *this;
}

Graph::Graph(string filename) {
    ifstream ifs{filename};
    for (string line; getline(ifs, line); line = "") {
        vector<string> vec = utilities::GetSubstrs(line, ',');
        data_.push_back(vec);
    }

    Node* lowest;
    for(unsigned int i = 1; i < data_.size(); i++) {
        struct Node* toInsert = new Node();
        toInsert->incidentID = data_[i][0];
        toInsert->totalLoss = stoi(data_[i][1]);
        toInsert->totalMigrants = stoi(data_[i][2]);
        pair<double, double> coor;
        // degrees to radians: degrees ⋅ pi / 180
        coor.first = stod(data_[i][3]) * M_PI / 180.0;
        coor.second = stod(data_[i][4]) * M_PI / 180.0;
        toInsert->coordinates = coor;
        if (i == 1) lowest == toInsert;
        if (calculateRisk(toInsert) < calculateRisk(lowest)) lowest = toInsert;
        vertices_.insert(toInsert);
        size++;
    }

    for (auto v : vertices_) { // populating edgeList
        vector<pair<Node*, double>> vect;
        for (auto e : vertices_) {
            if (calculateDistance(v, e) != 0) vect.push_back({e, calculateDistance(v, e)});
        }
        edgeList_[v] = vect;
    }
}

vector<Graph::Node*> Graph::getShortestPath(Node* node) {
    return short_paths_[node];
}

double Graph::calculateRisk(Node* node) { return node->totalLoss / node->totalMigrants; }

/**
 * φ = latitude
 * λ = longitude
 * R = Earth's radius = 6,371km
 * 
 * Haversine Formula:
 * a = sin²(Δφ/2) + cos(φ1) ⋅ cos(φ2) ⋅ sin²(Δλ/2)
 * c = 2 ⋅ atan2( √a, √(1−a) )
 * distance = R ⋅ c
*/
double Graph::calculateDistance(Node* one, Node* two){
    double a = pow((two->coordinates.first - one->coordinates.first)/2.0, 2.0) + cos(one->coordinates.first)
                * cos(two->coordinates.first) * pow((two->coordinates.second - one->coordinates.second)/2.0, 2.0);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return 6371 * c;
}

void Graph::dijkstra(Node* node) {
    priority_queue<pair<double, Node*>> pq;
 
    map<Node*, double> dist;
 
     pq.push({0, node});
     dist[node] = 0;

    while (!pq.empty()) {
        
        Node* u = pq.top().second;
        pq.pop();
 
        std::vector<pair<Node*, double>>::iterator i;
        for (i = edgeList_[u].begin(); i != edgeList_[u].end(); ++i) {
            // Get vertex label and weight of current
            // adjacent of u.
            Node* v = (*i).first;
            double weight = (*i).second;
 
            // If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight) {
                // Updating distance of v
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
                
                // add vertex to path from source to final node
                short_paths_[v].push_back(v);
            }
         }
    }
}

/*
     * Returns a vector of all nodes that have a loss ratio within 
     * a given range of a target
    */
std::vector<Node*> findByLoss(double target, double range) {
        // to return
        std::vector<Node*> out;
    
        // Mark all the vertices as not visited
        std::map<Node*, bool> visited;
        for (auto node : vertices_) visited.emplace(node, false);

        // Create a queue for BFS
        std::queue<Node*> q;
    
        // // Mark the current node as visited and enqueue it
        Node* current = lowest_risk_;
        visited[current] = true;
        q.push(current);
    
        while(!q.empty()) {
            // Dequeue a vertex from queue
            current = q.front();

            // check if current's risk is within range of target
            if (abs(target - calculateRisk(current)) <= range) out.push_back(current);
            // pop
            q.pop();
        
            // Get all adjacent vertices of the dequeued
            // vertex current. If a adjacent has not been visited,
            // then mark it visited and enqueue it
            for (auto adj : edgeList_[current]) {
                if (!visited[adj.first]) {
                    visited[adj.first] = true;
                    q.push(adj.first);
                }
            }
        }
        return out;
    }