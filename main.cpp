#include <iostream>
#include "Visualizer.h"
#include <cassert>
#include "Toolbar.h"

int main() {
    Graph g;
    std::shared_ptr<Node> v1 = g.addNode(200, 100);
    std::shared_ptr<Node> v2 = g.addNode(400, 100);
    std::shared_ptr<Node> v3 = g.addNode(300, 200);
    std::shared_ptr<Node> v4 = g.addNode(200, 300);
    std::shared_ptr<Node> v5 = g.addNode(600, 100);
    std::shared_ptr<Node> v6 = g.addNode(500, 200);
    std::shared_ptr<Node> v7 = g.addNode(400, 300);
    std::shared_ptr<Node> v8 = g.addNode(300, 400);
    std::shared_ptr<Node> v9 = g.addNode(700, 200);
    std::shared_ptr<Node> v10 = g.addNode(600, 300);
    std::shared_ptr<Node> v11 = g.addNode(500, 400);
    std::shared_ptr<Node> v12 = g.addNode(700, 400);

    std::shared_ptr<Edge> e1 = std::make_shared<Edge>(v1, v2);
    std::shared_ptr<Edge> e2 = std::make_shared<Edge>(v1, v3);
    std::shared_ptr<Edge> e3 = std::make_shared<Edge>(v1, v4);
    std::shared_ptr<Edge> e4 = std::make_shared<Edge>(v2, v3);
    std::shared_ptr<Edge> e5 = std::make_shared<Edge>(v2, v5);
    std::shared_ptr<Edge> e6 = std::make_shared<Edge>(v2, v6);
    std::shared_ptr<Edge> e7 = std::make_shared<Edge>(v3, v4);
    std::shared_ptr<Edge> e8 = std::make_shared<Edge>(v3, v6);
    std::shared_ptr<Edge> e9 = std::make_shared<Edge>(v3, v7);
    std::shared_ptr<Edge> e10 = std::make_shared<Edge>(v4, v7);
    std::shared_ptr<Edge> e11 = std::make_shared<Edge>(v4, v8);
    std::shared_ptr<Edge> e12 = std::make_shared<Edge>(v5, v6);
    std::shared_ptr<Edge> e13 = std::make_shared<Edge>(v5, v9);
    std::shared_ptr<Edge> e14 = std::make_shared<Edge>(v6, v7);
    std::shared_ptr<Edge> e15 = std::make_shared<Edge>(v6, v10);
    std::shared_ptr<Edge> e16 = std::make_shared<Edge>(v6, v11);
    std::shared_ptr<Edge> e17 = std::make_shared<Edge>(v6, v12);
    std::shared_ptr<Edge> e18 = std::make_shared<Edge>(v7, v10);
    std::shared_ptr<Edge> e19 = std::make_shared<Edge>(v7, v11);
    std::shared_ptr<Edge> e20 = std::make_shared<Edge>(v8, v10);
    std::shared_ptr<Edge> e21 = std::make_shared<Edge>(v8, v11);

    g.addEdge(e1);
    g.addEdge(e2);
    g.addEdge(e3);
    g.addEdge(e4);
    g.addEdge(e5);
    g.addEdge(e6);
    g.addEdge(e7);
    g.addEdge(e8);
    g.addEdge(e9);
    g.addEdge(e10);
    g.addEdge(e11);
    g.addEdge(e12);
    g.addEdge(e13);
    g.addEdge(e14);
    g.addEdge(e15);
    g.addEdge(e16);
    g.addEdge(e17);
    g.addEdge(e18);
    g.addEdge(e19);
    g.addEdge(e20);
    g.addEdge(e21);

    Visualizer algoViz(g);
    algoViz.run();

    return 0;
}
