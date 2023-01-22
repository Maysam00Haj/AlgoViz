#ifndef ALGOVIZ_VISUALIZER_H
#define ALGOVIZ_VISUALIZER_H
#include "Graph.h"
#include "Toolbar.h"


class Visualizer {
    enum vis_mode {BFS, DFS, MST, DIJKSTRA};
    sf::RenderWindow* window;
    sf::Event sfEvent;
    Graph graph;
    Toolbar toolbar;
    bool node_is_clicked = false;
    std::shared_ptr<Node> clicked_node;
    vis_mode mode = BFS;

    void initWindow();

public:
    Visualizer();
    ~Visualizer();
    void update();
    void render();
    void run();
    void executeClickAction(float pos_x, float pos_y);
    void runBFS();
    void runDFS();
    void runMST();
    void runDijkstra();
    void runAlgorithm();
};

#endif //ALGOVIZ_VISUALIZER_H
