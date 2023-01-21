#include "Edge.h"

Edge::Edge(const Node& node1, const Node& node2): node1(node1), node2(node2){
    float x1 = node1.getShape().getPosition().x;
    float y1 = node1.getShape().getPosition().y;
    float x2 = node2.getShape().getPosition().x;
    float y2 = node2.getShape().getPosition().y;
    this->line[0] = sf::Vertex(sf::Vector2f(x1, y1));
    this->line[1] = sf::Vertex(sf::Vector2f(x2, y2));
}

Node Edge::getNode1() const {
    return this->node1;
}

Node Edge::getNode2() const {
    return this->node2;
}


void Edge::render(sf::RenderTarget& target) {
    target.draw(this->line, 2, sf::Lines);
}

bool Edge::operator<(const Edge &other) const {
    return this->weight < other.weight;
}

bool Edge::operator==(const Edge &other) const {
    return this->weight == other.weight;
}
