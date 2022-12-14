#include "Node.h"
#include "cmath"
#include <utility>

#define RADIUS 30

Node::Node(std::string name, float pos_x, float pos_y): name(std::move(name)),neighbors(), shape(sf::CircleShape()) {
    setShape(pos_x, pos_y);
}

void Node::setShape(float pos_x, float pos_y) {
    this->shape.setRadius(RADIUS);
    this->shape.setOutlineColor(sf::Color::Black);
    this->shape.setPosition(pos_x, pos_y);
}

sf::CircleShape Node::getShape() const {
    return this->shape;
}

sf::Rect<float> Node::getShapeLocalBounds() const {
    return this->shape.getLocalBounds();
}

void Node::render(sf::RenderTarget& target) {
    target.draw(this->shape);
}

std::string Node::getName() const {
    return this->name;
}

bool Node::checkCollision(std::shared_ptr<Node> node) const {
    sf::Vector2<float> first_center = this->getShape().getPosition();
    sf::Vector2<float> second_center = node->getShape().getPosition();
    return abs(pow(first_center.x - second_center.x, 2) + pow(first_center.y - second_center.y, 2)) < pow(2 * RADIUS, 2);
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


