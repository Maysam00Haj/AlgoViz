#ifndef ALGOVIZ_VISUALIZER_H
#define ALGOVIZ_VISUALIZER_H
#include "Graph.h"
#include "Toolbar.h"


class Visualizer {
    sf::RenderWindow* window;
    sf::Event sfEvent;
    Graph graph;
    Toolbar toolbar;
    bool node_is_clicked = false;
    std::shared_ptr<Node> clicked_node;

    void initWindow();

public:
    Visualizer();
    ~Visualizer();
    void update();
    void render();
    void run();
    void executeClickAction(float pos_x, float pos_y);
};

#endif //ALGOVIZ_VISUALIZER_H
