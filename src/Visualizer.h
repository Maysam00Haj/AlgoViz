#ifndef ALGOVIZ_VISUALIZER_H
#define ALGOVIZ_VISUALIZER_H
#include "Graph.h"
#include "Interface.h"

enum VisMode {
    BFS,
    DFS,
    DIJKSTRA
};

class Visualizer {
public:
    explicit Visualizer(const Graph& graph = Graph());
    ~Visualizer();
    void run();

private:
    sf::RenderWindow* window;
    sf::Event sfEvent{};
    sf::View original_view;
    sf::View current_view;
    Graph graph;
    Toolbar toolbar;
    std::shared_ptr<Node> clicked_node;
    sf::Font* vis_font;
    bool node_is_clicked = false;
    float current_zoom_factor = 1.f;
    std::vector<std::string> saved_graphs;


    std::map<int, ButtonId> tutorial_stages {
            {0, CURSOR}, {1, ADD_NODE}, {2, ADD_NODE}, {3, ADD_NODE}, {4, ADD_EDGE},
            {5, ADD_EDGE}, {6, ADD_EDGE}, {7, CHANGE_START_NODE}, {8, CHOOSE_TARGET_NODE},
            {9, RUN_BFS}, {10, CLEAR_WINDOW}
    };


    static std::vector<std::shared_ptr<Node>> parseNodesFromString(const std::string& graph_literal, sf::Font* font);
    static std::vector<std::shared_ptr<Edge>> parseEdgesFromString(const std::string& graph_literal,
                                                                   std::vector<std::shared_ptr<Node>>& nodes);

    bool viewIsInBounds();

    void runTutorial();
    void update();
    void render();
    void executeClickAction();
    void runAlgorithm();
    void cursorRoutine();
    void addNodeRoutine();
    void addEdgeRoutine();
    void eraseRoutine();
    void changeStartNodeRoutine();
    void endRoutine();
    void resetRoutine();
    void clearWindowRoutine();
    void chooseTargetNodeRoutine();
    void removeTargetNodeRoutine();
    void saveToFile();
    void loadFromFile();
    void deleteFromFile();

};

#endif //ALGOVIZ_VISUALIZER_H
