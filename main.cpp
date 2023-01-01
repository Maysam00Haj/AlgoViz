#include <iostream>
#include "Node.h"
#include "Edge.h"
#include "Graph.h"
#include "SFML/Graphics.hpp"
#include <cassert>
#include "Toolbar.h"




int main() {
  /*  Node v1("v1");
    Node v2("v2");
    Node v3("v3");
    Node v4("v4");

    Edge e1(v1, v2);
    Edge e2(v1, v3);
    Edge e3(v4, v1);
    Edge e4(v2, v4);
    Edge e5(v3, v2);

    Graph graph;
    assert(graph.getStartNode() == nullptr);

    graph.addNode(v2);
    graph.addNode(v3);
    assert(graph.getNodesNum() == 2);

    graph.removeNode("v2");
    assert(graph.getNodesNum() == 1);

    graph.addNode(v1);
    assert(graph.getNodesNum() == 2);
    graph.addEdge(e2);
    assert(graph.getEdgesNum() == 1);

*/

    sf::RenderWindow window(sf::VideoMode(1000, 600), "Graph");
    //Button* button = new Button(200, 200, 100, 100, "C:\\Users\\Maysam\\Desktop\\eraserIcon.png", 0);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
            window.clear();
        //    button->render(window);
            window.display();
        }
    }

    return 0;
}
