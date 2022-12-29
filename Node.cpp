#include "Node.h"

#include <utility>

Node::Node(std::string name): name(std::move(name)),neighbors(), shape(sf::CircleShape()) {}

void Node::setShape(float pos_x, float pos_y) {
    this->shape.setRadius(30);
    this->shape.setOutlineColor(sf::Color::Black);
    this->shape.setPosition(pos_x, pos_y);
}

sf::CircleShape Node::getShape() {
    return this->shape;
}

std::string Node::getName() const {
    return this->name;
}

void Node::setDistance(int updated_distance) {
    this->distance = updated_distance;
}

int Node::getDistance() const {
    return this->distance;
}

void Node::setPathWeight(int updated_path_weight) {
    this->path_weight = updated_path_weight;
}

int Node::getPathWeight() const {
    return this->path_weight;
}

bool Node::operator==(const Node& node) const {
    return (this->getName() == node.getName());
}


