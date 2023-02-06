//
// Created by User on 12/23/2022.
//
#include <iostream>
#include <queue>
#include "Graph.h"
#include "Node.h"
#include <thread>
#include <mutex>

std::mutex m1;
extern bool algo_thread_is_running;


void Graph::render(sf::RenderTarget& target) {
    for (auto &node: this->nodes_list) {
        node.second->render(target);
        for (auto &edge: this->edges_list[node.first]) {
            edge->render(target);
        }
    }
}


void Graph::addNode(float pos_x, float pos_y) {
    std::string node_name = generateName();
    std::shared_ptr<Node> node_ptr = std::make_shared<Node>(node_name, pos_x, pos_y);
    if (!checkValidPos(*node_ptr)) return; //TODO: exception
    this->nodes_list[node_name] = node_ptr;
    this->neighbors_list[node_name] = {};
    this->nodes_num++;
    this->name_count++;
    if (this->nodes_num == 1) setStartNode(node_ptr);
}


void Graph::removeNode(const std::string& node_name) {
    if (this->nodes_list.find(node_name) == this->nodes_list.end()) return;
    bool start_node_isChanged;

    std::shared_ptr<Node> node_to_delete = this->nodes_list[node_name];

    // update the start node if it's the deleted node
    if (node_name == this->start_node->getName()) {
        start_node_isChanged = true;
    }

    // go over all the nodes' neighbors and delete the edges between them
    for (const std::shared_ptr<Edge>& edge : this->edges_list[node_name]) {
        if (edge->getNode1()->getName() == node_name) {
            this->edges_list[edge->getNode2()->getName()].erase(edge);
        }
        else {
            this->edges_list[edge->getNode1()->getName()].erase(edge);
        }
    }

    // erase the node from every neighboring node's list
    for (const std::shared_ptr<Node>& neighbor_node : this->neighbors_list[node_name]) {
        neighbors_list[neighbor_node->getName()].erase(node_to_delete);
    }

    this->nodes_list.erase(node_name);
    this->edges_list.erase(node_name); // delete edges list entry
    this->neighbors_list.erase(node_name); // erase the node's neighbors entry from hashmap
    if (start_node_isChanged) {
        if (this->nodes_list.empty()) this->start_node = nullptr;
        else setStartNode(nodes_list.begin()->second);
    }
    this->nodes_num--;
}


void Graph::addEdge(std::shared_ptr<Edge> edge) {
    if (this->containsEdge(edge)) return;
    std::shared_ptr<Node> node1 = edge->getNode1();
    std::shared_ptr<Node> node2 = edge->getNode2();
    this->edges_num++;
    this->edges_list[edge->getNode1()->getName()].insert(edge);
    this->edges_list[edge->getNode2()->getName()].insert(edge); // done twice because each edge exists in 2 lists, one for each node it connects
    this->neighbors_list[node1->getName()].insert(node2);
    this->neighbors_list[node2->getName()].insert(node1);
}


void Graph::removeEdge(const std::shared_ptr<Edge>& to_delete) {
    this->edges_list[to_delete->getNode1()->getName()].erase(to_delete);
    this->edges_list[to_delete->getNode2()->getName()].erase(to_delete);
    std::shared_ptr<Node> firstNode = to_delete->getNode1();
    std::shared_ptr<Node> secondNode = to_delete->getNode2();
    this->neighbors_list[firstNode->getName()].erase(secondNode);
    this->neighbors_list[secondNode->getName()].erase(firstNode);
    this->edges_num--;
}

const std::shared_ptr<Node>& Graph::getStartNode() const {
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

bool Graph::containsEdge(const std::shared_ptr<Edge>& edge) {
    std::string node1_name = edge->getNode1()->getName();
    std::string node2_name = edge->getNode2()->getName();

    if (this->edges_list.find(node1_name) == this->edges_list.end() ||
        this->edges_list.find(node2_name) == this->edges_list.end())
        return false;

    for (const auto& current_edge: edges_list[edge->getNode1()->getName()]) {
        if (current_edge->getNode1()->getName() == node1_name && current_edge->getNode2()->getName() == node2_name ||
            current_edge->getNode2()->getName() == node1_name && current_edge->getNode1()->getName() == node2_name)
            return true;
    }

    return false;
}

void Graph::setStartNode(const std::shared_ptr<Node>& new_start_node) {
    if (!new_start_node) return;
    if (this->start_node) this->start_node->setState(NODE_UNDISCOVERED);
    this->start_node = new_start_node;
    this->start_node->setDistance(0);
    this->start_node->setPathWeight(0);
    this->start_node->setState(NODE_START);
    this->start_node->setColor(sf::Color::Yellow);
}

bool Graph::hasNegativeCircle() {
    return false;
}

std::shared_ptr<Node> Graph::getNodeByPosition(float pos_x, float pos_y) {
    for (const auto& node: this->nodes_list) {
        if (node.second->getShape().getGlobalBounds().contains(pos_x, pos_y)) {
            return node.second;
        }
    }
    return nullptr;
}

std::shared_ptr<Edge> Graph::getEdgeByPosition(float pos_x, float pos_y) {
    for (const auto& edges: edges_list) {
        for (const auto& edge: edges.second) {
            if (edge->getShape().getGlobalBounds().contains(pos_x, pos_y)) {
                return edge;
            }
        }
    }
    return nullptr;
}

void Graph::runBFS(sf::RenderWindow& window) {
    algo_thread_is_running = true;
    m1.lock();
    window.setActive(true);
    if (!this->start_node) return;

    std::queue<std::shared_ptr<Node>> bfs_q;
    std::shared_ptr<Node> previous_node;
    std::shared_ptr<Edge> current_edge;
    bfs_q.push(this->start_node);
    bfs_q.front()->setState(NODE_CURRENT);
    this->renderAndWait(window);

    while (!bfs_q.empty()) {
        previous_node = bfs_q.front();
        if (previous_node->getState() == NODE_DONE) {
            bfs_q.pop();
            continue;
        }
        previous_node->setState(NODE_CURRENT);
        for (const std::shared_ptr<Node>& current_node: this->neighbors_list[previous_node->getName()]) {
            current_edge = getEdgeByNodes(previous_node, current_node);
            current_edge->setState(EDGE_DISCOVERED);
            if (current_node->getState() != NODE_DONE) {
                bfs_q.push(current_node);
                current_node->setState(NODE_DISCOVERED);
                current_node->setDistance(1);
                this->renderAndWait(window);
            }
        }
        previous_node->setState(NODE_DONE);
        bfs_q.pop();
        this->renderAndWait(window);
    }
    window.setActive(false);
    m1.unlock();
}

void Graph::runDFS(sf::RenderTarget& target) {

}

void Graph::runMST(sf::RenderTarget& target) {

}

void Graph::runDijkstra(sf::RenderTarget& target) {

}

void Graph::reset() {
    for (const auto& node: nodes_list) {
        if (node.second == this->start_node) {
            node.second->setState(NODE_START);
        }
        else {
            node.second->setState(NODE_UNDISCOVERED);
        }
    }

    for (const auto& node: edges_list) {
        for (const auto& edge: node.second) {
            edge->setState(EDGE_UNDISCOVERED);
        }
    }
}

//**********************************************************************************************************************
// private helper functions:
void Graph::renderAndWait(sf::RenderWindow& window, bool wait) {
    window.clear();
    this->render(window);
    window.display();
    if (wait) {
        window.setActive(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        window.setActive(true);
    }
}

std::string Graph::generateName() const {
    return "node_" + std::to_string(this->name_count);
}

bool Graph::checkValidPos(const Node& node) const {
    for (auto& pair : this->nodes_list) {
        if (node.checkCollision(pair.second)) return false;
    }
    //Todo: set x and y limits
    return true;
}


std::shared_ptr<Edge> Graph::getEdgeByNodes(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) {
    for (const auto& edge: this->edges_list[node1->getName()]) {
        if (edge->getNode2()->getName() == node2->getName() || edge->getNode1()->getName() == node2->getName())
            return edge;
    }
    return nullptr;
}
