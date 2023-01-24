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
#include <SFML/Graphics.hpp>
#include <thread>

class Graph {
    int nodes_num = 0;
    int edges_num = 0;
    int name_count = 0;
    std::shared_ptr<Node> start_node = nullptr;
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes_list; // enables us to find node pointer by its name string
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<Node>>> neighbors_list;
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<Edge>>> edges_list; // for each node there's a set of edges that touch it (2 copies of each edge)
    bool directed = false;

    std::string generateName() const;
    bool checkValidPos(const Node& node) const;

public:
    explicit Graph() = default;
    ~Graph() = default;
    void setStartNode(const std::shared_ptr<Node>& newStartNode);
    void render(sf::RenderTarget& target);
    void addNode(float pos_x, float pos_y);
    void removeNode(const std::string& node_name);
    void addEdge(const std::shared_ptr<Edge>& edge);
    void removeEdge(const std::shared_ptr<Edge>& edge);
    std::shared_ptr<Node> getStartNode() const;
    int getNodesNum() const;
    int getEdgesNum() const;
    bool containsNode(const std::string& node_name);
    bool containsEdge(const std::shared_ptr<Edge>& edge);
    bool hasNegativeCircle();
    std::shared_ptr<Node> getNodeByPosition(float pos_x, float pos_y);
    std::shared_ptr<Edge> getEdgeByPosition(float pos_x, float pos_y);
    void runBFS();
    void runDFS();
    void runMST();
    void runDijkstra();
};
#endif //ALGOVIZ_GRAPH_H
