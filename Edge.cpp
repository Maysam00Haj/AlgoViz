#include "Edge.h"
#include <cmath>

static float getAngle(float x1, float y1, float x2, float y2) {
    float rads = std::atan((y2-y1) / (x2-x1));
    float degrees = (rads * 180)/(float)std::numbers::pi;
    float angle = degrees - 90; // correction for shape::setRotation function
    if (x1 > x2) angle += 180; // another correction
    return angle;
}

static float getDistance(float x1, float y1, float x2, float y2) {
    auto distance = (float)sqrt(pow(x2-x1,2) + pow(y2-y1,2));
    return distance;
}

Edge::Edge(const Node& node1, const Node& node2): node1(node1), node2(node2){
    float x1 = node1.getShape().getPosition().x+30;
    float y1 = node1.getShape().getPosition().y+30;
    float x2 = node2.getShape().getPosition().x+30;
    float y2 = node2.getShape().getPosition().y+30;
    this->shape.setPosition(x1, y1);
    this->shape.setRotation(getAngle(x1,y1,x2,y2));
    this->shape.setSize(sf::Vector2f(3, getDistance(x1,y1,x2,y2)));
}

Node Edge::getNode1() const {
    return this->node1;
}

Node Edge::getNode2() const {
    return this->node2;
}

sf::RectangleShape Edge::getShape() const {
    return this->shape;
}

void Edge::render(sf::RenderTarget& target) {
    target.draw(this->shape);
}

bool Edge::operator<(const Edge &other) const {
    return this->weight < other.weight;
}

bool Edge::operator==(const Edge &other) const {
    return this->weight == other.weight;
}

