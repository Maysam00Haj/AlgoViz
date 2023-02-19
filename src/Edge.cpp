#include "Edge.h"
#include "utils.h"



Edge::Edge(const std::shared_ptr<Node>& first_node, const std::shared_ptr<Node>& second_node, bool do_correct): first_node(first_node), second_node(second_node) {
    std::vector<float> coordinates = correctEdgeCoordinates();
    if(!do_correct) {
        this->length = getDistance(coordinates[0], coordinates[1], coordinates[2], coordinates[3]) - NODE_RADIUS;
        this->shape.setSize(sf::Vector2f(3, this->length));
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
    return this->length < other->length;
}

bool Edge::operator==(const std::shared_ptr<Edge> &other) const {
    return this->length == other->length;
}

void Edge::setColor(const sf::Color &color) {
    this->shape.setFillColor(color);
}

float Edge::getLength() const {
    return this->shape.getSize().y;

}

void Edge::setState(EdgeState state) {
    switch (state) {
        case EDGE_UNDISCOVERED: {
            this->setColor(UNDISCOVERED_EDGE_COLOR);
            break;
        }
        case EDGE_DISCOVERED: {
            this->setColor(DISCOVERED_EDGE_COLOR);
            break;
        }
        case EDGE_NEAREST: {
            this->setColor(NEAREST_EDGE_COLOR);
            break;
        }
        case EDGE_SELECTED: {
            this->setColor(SELECTED_EDGE_COLOR);
            break;
        }
        default: {
            break;
        }
    }
}

std::vector<float> Edge::correctEdgeCoordinates() {
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
    this->length = getDistance(x1, y1, x2, y2) - 2 * NODE_RADIUS;
    return {x1, y1, x2, y2};
}

