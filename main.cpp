#include <iostream>
#include "Node.h"
#include "SFML/Graphics.hpp"



int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Graph");
    Node* node = new Node("v");

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    node->setShape(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                    break;
            }

            window.draw(node->getShape());
            window.display();
        }
    }
    return 0;
}
