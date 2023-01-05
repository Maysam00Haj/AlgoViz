#ifndef ALGOVIZ_VISUALIZER_H
#define ALGOVIZ_VISUALIZER_H
#include "Graph.h"
#include "Toolbar.h"

class Visualizer {
private:
    sf::RenderWindow* window;
    sf::Event sfEvent;
    Graph graph;
  //Toolbar toolbar; //Todo: fix

    void initWindow();
    void initGraph();
    void initToolbar();
    void initState();

public:
    Visualizer();
    ~Visualizer();
    void update();
    void render();
    void run();
};

#endif //ALGOVIZ_VISUALIZER_H
