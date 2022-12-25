//
// Created by User on 12/23/2022.
//
#ifndef ALGOVIZ_GRAPH_H
#define ALGOVIZ_GRAPH_H

#include <string>
#include <unordered_map>
#include "Node.h"
#include "Edge.h"


class Graph {
    Node start_node;
    unordered_map<string,unordered_map<string,Node>> neighbors_list;

public:
    explicit Graph(Node start_node);
    ~Graph() = default;
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    void addNode(const Node& node);
    void removeNode(string node_name);
    void addEdge(const Edge& edge);
    void runBFS();
    void runDFS();
    void runDijkstra();

};
#endif //ALGOVIZ_GRAPH_H
