//
// Created by User on 12/23/2022.
//
#include "Graph.h"
#include "Node.h"
#include "SFML/Graphics.hpp"


void Graph::addNode(const Node& node) { //TODO: check if the node already exists and handle accordingly
    std::string node_name = node.getName();
    if (this->nodes_list.find(node_name) != this->nodes_list.end()) return;

    this->nodes_list[node_name] = std::make_shared<Node>(node);
    this->neighbors_list[node_name] = {};
    this->nodes_num++;
}


void Graph::removeNode(const std::string& node_name) { //TODO: is there a need to handle the case where the node doesn't exist?
    if (this->nodes_list.find(node_name) == this->nodes_list.end()) return;

    this->nodes_list.erase(node_name);
    this->nodes_num--;

    // go over all the nodes' neighbors and delete the edges between them
    for (const auto& neighbor : this->neighbors_list[node_name]) { // for every neighbor of the node with the given name
        for (const auto& edge : this->edges_list[neighbor->getName()]) { // for every edge touching the neighbor
            if (edge->getNode1().getName() == node_name || edge->getNode2().getName() == node_name) { // if it also touches the node with the given name
                this->edges_list[neighbor->getName()].erase(edge); // delete the edge
                this->edges_num--;
            }
        }
    }

    this->edges_list.erase(node_name); // delete edges list entry

    this->neighbors_list.erase(node_name); // erase the node's neighbors entry from hashmap

    // erase the node from every neighboring node's list
    for (auto neighbors: neighbors_list) { // tried to use structured binding which requires c++20, but didn't work for some reason
        for (const auto& node : neighbors.second) {
            if (node->getName() == node_name) neighbors.second.erase(node);
        }
    }
}


void Graph::addEdge(const Edge& edge) {
    this->edges_num++;
    this->edges_list[edge.getNode1().getName()].insert(std::make_shared<Edge>(edge));
    this->edges_list[edge.getNode2().getName()].insert(std::make_shared<Edge>(edge)); // done twice because each edge exists in 2 lists, one for each node it connects
}


void Graph::removeEdge(const Edge& to_delete) {
    this->edges_num--;
    for (const auto& edge : this->edges_list[to_delete.getNode1().getName()]) {
        if (edge->getNode1().getName() == to_delete.getNode1().getName())
            this->edges_list[to_delete.getNode1().getName()].erase(edge);
    }

    for (const auto& edge : this->edges_list[to_delete.getNode2().getName()]) {
        if (edge->getNode2().getName() == to_delete.getNode2().getName())
            this->edges_list[to_delete.getNode2().getName()].erase(edge);
    }
}

std::shared_ptr<Node> Graph::getStartNode() const {
    return this->start_node;
}

int Graph::getNodesNum() const {
    return nodes_num;
}

int Graph::getEdgesNum() const {
    return edges_num;
}

bool Graph::containsNode(const std::string& node_name) {
    return (this->nodes_list.find(node_name) != this->nodes_list.end());
}


bool Graph::containsEdge(const Edge& edge) {
    std::string node1_name = edge.getNode1().getName();
    std::string node2_name = edge.getNode2().getName();

    if (this->edges_list.find(node1_name) == this->edges_list.end() ||
        this->edges_list.find(node2_name) == this->edges_list.end())
        return false;

    return (this->edges_list[node1_name].find(std::make_shared<Edge>(edge)) == this->edges_list[node1_name].end());
}



void Graph::changeStartNode(const Node& new_start_node) { //TODO: implement containsNode and containsEdge and change this implementation
    if (!this->containsNode(new_start_node.getName())) return;
    this->start_node = std::make_shared<Node>(new_start_node);
}


void Graph::runBFS() {

}

void Graph::runDFS() {

}

void Graph::runDijkstra() {

}

bool Graph::hasNegativeCircle() {
    return false;
}
