#ifndef ALGOVIZ_VISUALIZER_H
#define ALGOVIZ_VISUALIZER_H
#include "Graph.h"
#include "Toolbar.h"

enum VisMode {BFS, DFS, MST, DIJKSTRA};

class Visualizer {
    sf::RenderWindow* window;
    sf::Event sfEvent{};
    Graph graph;
    Toolbar toolbar;
    bool node_is_clicked = false;
    std::shared_ptr<Node> clicked_node;
    VisMode mode = BFS;

public:
    explicit Visualizer(const Graph& graph = Graph());
    ~Visualizer();
    void update();
    void render();
    void run();
    void executeClickAction();
    void runAlgorithm();
};

#endif //ALGOVIZ_VISUALIZER_H
