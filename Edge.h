//
// Created by User on 12/19/2022.
//
#ifndef ALGOVIZ_EDGE_H
#define ALGOVIZ_EDGE_H

#include "Node.h"
#include "SFML/Graphics.hpp"

#define UNDISCOVERED_EDGE_COLOR (sf::Color(192, 192, 192, 70))
#define DISCOVERED_EDGE_COLOR   (sf::Color::White)
#define NODE_RADIUS 30

enum EdgeState {EDGE_DISCOVERED, EDGE_UNDISCOVERED};

class Edge {
    EdgeState state = EDGE_UNDISCOVERED;
    const std::shared_ptr<Node> first_node;
    const std::shared_ptr<Node> second_node;
    sf::RectangleShape shape;
    std::string literal;
    int weight = 0;

public:
    explicit Edge(std::shared_ptr<Node> first_node, std::shared_ptr<Node> second_node);
    ~Edge() = default;
    std::shared_ptr<Node> getFirstNode() const;
    std::shared_ptr<Node> getSecondNode() const;
    sf::RectangleShape getShape() const;
    void render(sf::RenderTarget& target);
    void setColor(const sf::Color& color);
    sf::Color getColor() const;
    void setState(EdgeState state);
    EdgeState getState() const;
    bool operator<(const std::shared_ptr<Edge>& other) const;
    bool operator==(const std::shared_ptr<Edge>& other) const;
};

#endif //ALGOVIZ_EDGE_H
