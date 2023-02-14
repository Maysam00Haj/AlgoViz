#include "Edge.h"
#include "utils.h"



Edge::Edge(const std::shared_ptr<Node>& first_node, const std::shared_ptr<Node>& second_node, bool do_correct): first_node(first_node), second_node(second_node) {
    float x1 = first_node->getShape().getPosition().x + NODE_RADIUS;
    float y1 = first_node->getShape().getPosition().y + NODE_RADIUS;
    float x2 = second_node->getShape().getPosition().x + NODE_RADIUS;
    float y2 = second_node->getShape().getPosition().y + NODE_RADIUS;
    float rotation_angle = getAngle(x1, y1, x2, y2);
    float corrected_x1 = getCorrectedX1(x1, rotation_angle);
    float corrected_y1 = getCorrectedY1(y1, rotation_angle);
    this->shape.setRotation(rotation_angle);
    this->shape.setPosition(corrected_x1, corrected_y1);
    if (do_correct) {
        this->shape.setSize(sf::Vector2f(3, getDistance(x1, y1, x2, y2) - 2 * NODE_RADIUS));
    }
    else {
        this->shape.setSize(sf::Vector2f(3, getDistance(x1, y1, x2, y2) - NODE_RADIUS));
    }
    this->shape.setFillColor(UNDISCOVERED_EDGE_COLOR);
}

std::shared_ptr<Node> Edge::getFirstNode() const {
    return this->first_node;
}

std::shared_ptr<Node> Edge::getSecondNode() const {
    return this->second_node;
}

sf::RectangleShape Edge::getShape() const {
    return this->shape;
}

void Edge::render(sf::RenderTarget& target) {
    target.draw(this->shape);
}

bool Edge::operator<(const std::shared_ptr<Edge> &other) const {
    return this->weight < other->weight;
}

bool Edge::operator==(const std::shared_ptr<Edge> &other) const {
    return this->weight == other->weight;
}

void Edge::setColor(const sf::Color &color) {
    this->shape.setFillColor(color);
}

sf::Color Edge::getColor() const {
    return this->shape.getFillColor();
}

EdgeState Edge::getState() const {
    return this->state;
}

int Edge::getWeight() const {
    return this->weight;
}

void Edge::setState(EdgeState state) {
    this->state = state;
    switch (state) {
        case EDGE_UNDISCOVERED: {
            this->setColor(UNDISCOVERED_EDGE_COLOR);
            break;
        }
        case EDGE_DISCOVERED: {
            this->setColor(DISCOVERED_EDGE_COLOR);
            break;
        }
        case EDGE_SELECTED: {
            this->setColor(SELECTED_EDGE_COLOR);
        }
        default: {
            break;
        }
    }
}

void Edge::setWeight(int weight) {
    this->weight = weight;
}

void Edge::correctEdgeCoordinates() {
    float x1 = this->first_node->getShape().getPosition().x + NODE_RADIUS;
    float y1 = this->first_node->getShape().getPosition().y + NODE_RADIUS;
    float x2 = this->second_node->getShape().getPosition().x + NODE_RADIUS;
    float y2 = this->second_node->getShape().getPosition().y + NODE_RADIUS;
    float rotation_angle = getAngle(x1, y1, x2, y2);
    float corrected_x1 = getCorrectedX1(x1, rotation_angle);
    float corrected_y1 = getCorrectedY1(y1, rotation_angle);
    this->shape.setRotation(rotation_angle);
    this->shape.setPosition(corrected_x1, corrected_y1);
    this->shape.setSize(sf::Vector2f(3, getDistance(x1, y1, x2, y2) - 2 * NODE_RADIUS));
}

void Edge::toggle() {
    this->is_toggled = true;
}

void Edge::untoggle() {
    this->is_toggled = false;
}

