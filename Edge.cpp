#include "Edge.h"
#include <cmath>

#define NODE_RADIUS 30

static float getAngle(float x1, float y1, float x2, float y2);
static float getDistance(float x1, float y1, float x2, float y2);
static float getCorrectedX1(float x1, float x2, float angle);
static float getCorrectedY1(float y1, float y2, float angle);



Edge::Edge(const Node& node1, const Node& node2): node1(node1), node2(node2){
    float x1 = node1.getShape().getPosition().x+NODE_RADIUS;
    float y1 = node1.getShape().getPosition().y+NODE_RADIUS;
    float x2 = node2.getShape().getPosition().x+NODE_RADIUS;
    float y2 = node2.getShape().getPosition().y+NODE_RADIUS;
    float rotation_angle = getAngle(x1,y1,x2,y2);
    float corrected_x1 = getCorrectedX1(x1,x2,rotation_angle);
    float corrected_y1 = getCorrectedY1(y1,y2,rotation_angle);
    this->shape.setRotation(rotation_angle);
    this->shape.setPosition(corrected_x1, corrected_y1);
    this->shape.setSize(sf::Vector2f(3, getDistance(x1,y1,x2,y2) - 2*NODE_RADIUS));
    this->shape.setFillColor(sf::Color::Green);
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

void Edge::setColor(const sf::Color &color) {
    this->shape.setFillColor(color);
}

sf::Color Edge::getColor() const {
    return this->shape.getFillColor();
}

Edge::edge_state Edge::getState() const {
    return this->state;
}

void Edge::setState(Edge::edge_state state) {
    this->state = state;
}

//**********************************************************************************************************************


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

static float getCorrectedX1(float x1, float x2, float angle) {
    float rads = ((angle+90) * (float)std::numbers::pi) / 180;
    float offset = NODE_RADIUS * std::cos(rads);
    return x1 + offset;
}

static float getCorrectedY1(float y1, float y2, float angle) {
    float rads = ((angle+90) * (float)std::numbers::pi) / 180;
    float offset = NODE_RADIUS * std::sin(rads);
    return y1 + offset;
}

