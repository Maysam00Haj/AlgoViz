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
#include "Toolbar.h"
#include <SFML/Graphics.hpp>
#include <thread>


#define BG_COLOR (sf::Color(0,0,50,255))

class Graph {
    int nodes_num = 0;
    int edges_num = 0;
    int name_count = 0;
    bool directed = false;

    std::shared_ptr<Node> start_node = nullptr;
    std::shared_ptr<Node> target_node = nullptr;
    std::shared_ptr<Node> toggled_node = nullptr;
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes_list; // enables us to find node pointer by its name string
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<Node>>> neighbors_list;
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<Edge>>> edges_list; // for each node there's a set of edges that touch it (2 copies of each edge)

    void renderAndWait(sf::RenderWindow& window, Toolbar& toolbar, sf::View original_view, sf::View current_view, sf::Font* font, bool wait = true);
    bool dfs(const std::shared_ptr<Node>& prev, const std::shared_ptr<Node>& start, sf::RenderWindow& window, Toolbar& toolbar, sf::View& original_view, sf::View& current_view, sf::Font* font, bool wait = false);
    std::shared_ptr<Node> dijkstraMinDistance() const;
    std::string generateNodeName() const;
    void calculate_distances();

public:
    explicit Graph() = default;
    Graph(const Graph& other);
    ~Graph() = default;
    void setStartNode(const std::shared_ptr<Node>& newStartNode);
    void setTargetNode(const std::shared_ptr<Node>& target_node);
    void render(sf::RenderWindow& target, sf::Font* font);
    void addNode(std::shared_ptr<Node>& node);
    void removeNode(const std::string& node_name);
    void addEdge(std::shared_ptr<Edge>& edge);
    void removeEdge(const std::shared_ptr<Edge>& edge);
    void removeTargetNode();
    bool containsEdge(const std::shared_ptr<Edge>& edge);
    void runBFS(sf::RenderWindow& window, Toolbar& toolbar, sf::View& original_view, sf::View& current_view, sf::Font* font, bool wait = false);
    void runDFS(sf::RenderWindow& window, Toolbar& toolbar, sf::View& original_view, sf::View& current_view, sf::Font* font, bool wait = false);
    void runDijkstra(sf::RenderWindow& window, Toolbar& toolbar, sf::View& original_view, sf::View& current_view, sf::Font* font, bool wait = false);
    void reset();
    void setToggledNode(std::shared_ptr<Node>& to_toggle);
    void untoggle();
    bool checkValidPosition(const Node& node) const;
    const std::shared_ptr<Node>& getStartNode() const;
    std::shared_ptr<Node> getNodeByPosition(float pos_x, float pos_y);
    std::shared_ptr<Edge> getEdgeByPosition(float pos_x, float pos_y);
    std::shared_ptr<Edge> getEdgeByNodes(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2);
    std::shared_ptr<Node> addNode(float pos_x, float pos_y, sf::Font* text_font);
    std::shared_ptr<Node> getCollidedNode(const std::shared_ptr<Node>& moving_node) const;
    std::string getEncoding();
};
#endif //ALGOVIZ_GRAPH_H
