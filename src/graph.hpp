//
//  Created by Zihang Wen on 11/13/2025.
//  Copyright © 2025 Zihang Wen. All rights reserved.
//

#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>

using namespace std;

class Graph {
public:
    int popsize;
    std::vector<int> degrees;
    std::vector<std::vector<int>> edgelist;
    Graph(string);
};

Graph::Graph(std::string input_name) {
    std::ifstream input(input_name);
    if (!input) {
        throw std::runtime_error("Could not open file: " + input_name);
    }

    std::vector<std::pair<int, int>> edges;
    int node1, node2;
    popsize = 0;

    // Read all edges, track max node index
    while (input >> node1 >> node2) {
        popsize = std::max(popsize, std::max(node1, node2));
        edges.emplace_back(node1, node2);
    }

    // Nodes are assumed to be 0..popsize
    ++popsize;

    // Initialize degrees and adjacency list
    degrees.assign(popsize, 0);
    edgelist.assign(popsize, std::vector<int>{});

    // Build undirected adjacency list and degrees
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;

        if (u == v) {
            edgelist[u].push_back(v);
            ++degrees[u];
        } else {
            edgelist[u].push_back(v);
            edgelist[v].push_back(u);

            ++degrees[u];
            ++degrees[v];
        }
    }
}
