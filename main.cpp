#include <iostream>
#include "Node.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Graph");
    Node* node = new Node('a');

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    std::cout << event.mouseButton.x << "   " << event.mouseButton.y << std::endl;
                    node->setShape(event.mouseButton.x, event.mouseButton.y);
                    break;
            }

            window.clear({100,100,100}); //grey
            window.draw(node->getShape());
            window.display();
        }
    }
    return 0;
}
