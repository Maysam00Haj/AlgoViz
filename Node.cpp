#include "Node.h"

Node::Node(string name)  {
    this->name = name;
    this->neighbors = vector<Node*>();
    this->shape = sf::CircleShape(); // Creates a circle with radius = 0.

}

void Node::setShape(float pos_x, float pos_y) {
    this->shape.setRadius(30);
    this->shape.setOutlineColor(sf::Color::Black);
    this->shape.setPosition(pos_x, pos_y);
}

sf::CircleShape Node::getShape() {
    return this->shape;
}


