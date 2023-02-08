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
