#include "Visualizer.h"

void Visualizer::initWindow() {
    this->window = new sf::RenderWindow(sf::VideoMode(1000, 600), "Graph Visualizer");
    this->window->setFramerateLimit(60);
}

void Visualizer::initState() {

}

Visualizer::Visualizer() {
    this->initWindow();
    this->initState();
}

Visualizer::~Visualizer() {
  delete this->window;
}

void Visualizer::update() {
    while (this->window->pollEvent(this->sfEvent)) {
        switch (this->sfEvent.type) {
            case sf::Event::Closed:
                this->window->close();
            case sf::Event::MouseButtonPressed:
                if (this->sfEvent.mouseButton.button == sf::Mouse::Left) {
                    //Todo: work according to current state
                    this->graph.addNode(this->sfEvent.mouseButton.x, this->sfEvent.mouseButton.y);
                }
            case sf::Event::Resized:
//                window->setView(sf::View(sf::FloatRect(0, 0, this->sfEvent.size.width, this->sfEvent.size.height)));
            default:
                break;
        }
    }
}

void Visualizer::render() {
    this->window->clear();
    this->toolbar.render(*this->window);
    this->graph.render(*this->window);
    this->window->display();
}

void Visualizer::run() {
    while (this->window->isOpen()) {
        this->update();
        this->render();
    }
}