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
    std::vector<sf::RectangleShape> userInput;



    void update();
    void render();
    void executeClickAction();
    void runAlgorithm();
    void cursorRoutine();
    void addNodeRoutine();
    void addEdgeRoutine();
    void eraseRoutine();
    void changeStartNodeRoutine();
    void changeEdgeWeightRoutine();
    void runBFSRoutine();
    void runDfSRoutine();
    void runDijkstraRoutine();
    void endRoutine();
    void resetRoutine();
    void clearWindowRoutine();

public:
    explicit Visualizer(const Graph& graph = Graph());
    ~Visualizer();
    void run();
};

#endif //ALGOVIZ_VISUALIZER_H
