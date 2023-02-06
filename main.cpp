#include <iostream>
#include "Visualizer.h"
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


    float windowHeight = 1000;
    float windowWidth = 400;

    sf::RenderWindow window(sf::VideoMode(1000, 600), "Graph Visualizer");

    sf::Text* text = new sf::Text();
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) std::cout << "Couldn't load font file!" << std::endl;
    text->setFont(font);
    text->setFillColor(sf::Color::Yellow);
    text->setCharacterSize(23);
    text->setStyle(sf::Text::Bold | sf::Text::Underlined);
    text->setPosition(500,10);
    text->setString("33");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(*text);
        window.display();
    }


*/

    Visualizer algoViz;
    algoViz.run();

    return 0;
}
