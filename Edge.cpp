#include "Edge.h"

#include <utility>

Edge::Edge(Node node1, Node node2): node1(std::move(node1)), node2(std::move(node2)){}

Node Edge::getNode1() const {
    return this->node1;
}

Node Edge::getNode2() const {
    return this->node2;
}

void Edge::setShape(float pos_x, float pos_y, float pos_x2, float pos_y2) {

}

sf::RectangleShape Edge::getShape() const {
    return this->shape;
}