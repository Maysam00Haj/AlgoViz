#include "Node.h"
#include "cmath"
#include <utility>
#include "utils.h"
#include <iostream>

#define RADIUS 30

Node::Node(std::string name, float pos_x, float pos_y, sf::Font* text_font): name(std::move(name)) {
    this->shape.setRadius(RADIUS);
    this->shape.setOutlineColor(NODE_OUTLINE_COLOR);
    this->shape.setPosition(pos_x, pos_y);
}

sf::CircleShape Node::getShape() const {
    return this->shape;
}

void Node::render(sf::RenderWindow& window, sf::Font* font) {
    window.draw(this->shape);
    if (this->state != NODE_DONE && this->state != NODE_DISCOVERED && this->state != NODE_NEAREST && this->state != NODE_TARGET) return;
    sf::Text t;
    t.setFont(*font);
    t.setPosition(this->getPosition().x+21, this->getPosition().y+9);
    t.setFillColor(sf::Color::White);
    t.setCharacterSize(30);
    if (this->distance == INT_MAX)
        t.setString("inf");
    else
        t.setString(std::to_string(this->distance));
    window.draw(t);
}

std::string Node::getName() const {
    return this->name;
}

bool Node::checkBoundsCollision(const std::shared_ptr<Node>& node) const {
    sf::Vector2<float> first_center = this->getShape().getPosition();
    sf::Vector2<float> second_center = node->getShape().getPosition();
    return std::abs(pow(first_center.x - second_center.x, 2) + pow(first_center.y - second_center.y, 2)) < pow(2 * RADIUS, 2);
}

void Node::setDistance(int updated_distance) {
    this->distance = updated_distance;
}

int Node::getDistance() const {
    return this->distance;
}

bool Node::operator==(const std::shared_ptr<Node>& node) const {
    return (this->getName() == node->getName());
}

void Node::setColor(const sf::Color &color) {
    this->shape.setFillColor(color);
}


void Node::setState(NodeState state) {
    this->state = state;
    switch (this->state) {
        case NODE_START: {
            this->setColor(START_NODE_COLOR);
            break;
        }
        case NODE_TARGET: {
            this->setColor(TARGET_NODE_COLOR);
            break;
        }
        case NODE_NEAREST: {
            this->setColor(NEAREST_NODE_COLOR);
            break;
        }
        case NODE_CURRENT: {
            this->setColor(CURRENT_NODE_COLOR);
            break;
        }
        case NODE_DISCOVERED: {
            this->setColor(DISCOVERED_NODE_COLOR);
            break;
        }
        case NODE_UNDISCOVERED: {
            this->setColor(UNDISCOVERED_NODE_COLOR);
            break;
        }
        case NODE_DONE: {
            this->setColor(DONE_NODE_COLOR);
            break;
        }
        default: {
            break;
        }
    }
}

NodeState Node::getState() const {
    return this->state;
}


sf::Vector2f Node::getPosition() const {
    return {this->shape.getPosition().x, this->shape.getPosition().y};
}

void Node::setPosition(float pos_x, float pos_y) {
    this->shape.setPosition(pos_x, pos_y);
}


void Node::toggle() {
    int red = this->shape.getFillColor().r;
    int green = this->shape.getFillColor().g;
    int blue = this->shape.getFillColor().b;
    this->shape.setOutlineThickness(3);
    this->shape.setFillColor(sf::Color(red, green, blue, 150));
}

void Node::untoggle() {
    int red = this->shape.getFillColor().r;
    int green = this->shape.getFillColor().g;
    int blue = this->shape.getFillColor().b;
    this->shape.setOutlineThickness(0);
    this->shape.setFillColor(sf::Color(red, green, blue, 255));
}

void Node::setParent(const std::shared_ptr<Node>& parent) {
    this->parent_node = parent;
}

std::shared_ptr<Node> Node::getParent() const {
    return this->parent_node;
}




