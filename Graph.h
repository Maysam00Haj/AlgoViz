//
// Created by User on 12/23/2022.
//
#ifndef ALGOVIZ_GRAPH_H
#define ALGOVIZ_GRAPH_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "Node.h"
#include "Edge.h"


class Graph {
    int nodes_num = 0;
    int edges_num = 0;
    std::shared_ptr<Node> start_node = nullptr;
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes_list;
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<Node>>> neighbors_list; // enables us to find node pointer by its name string
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<Edge>>> edges_list; // for each node there's a set of edges that touch it (2 copies for each edge)
    bool directed = false; //TODO: is this the best way to handle different types of graphs?

public:
    explicit Graph() = default;
    ~Graph() = default;
    void addNode(const Node& node);
    void removeNode(const std::string& node_name);
    void addEdge(const Edge& edge);
    void removeEdge(const Edge& edge);
    std::shared_ptr<Node> getStartNode() const;
    int getNodesNum() const;
    int getEdgesNum() const;
    bool containsNode(const std::string& node_name); //important, changes some implementation
    bool containsEdge(const Edge& edge);//same as above
    void changeStartNode(const Node& new_start_node);
    void runBFS();
    void runDFS();
    void runDijkstra();
    bool hasNegativeCircle();

};
#endif //ALGOVIZ_GRAPH_H
