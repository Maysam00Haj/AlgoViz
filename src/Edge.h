//
// Created by User on 12/19/2022.
//
#ifndef ALGOVIZ_EDGE_H
#define ALGOVIZ_EDGE_H

#include "Node.h"
#include "SFML/Graphics.hpp"

#define UNDISCOVERED_EDGE_COLOR (sf::Color(150, 150, 150, 70))
#define DISCOVERED_EDGE_COLOR   (sf::Color::White)
#define SELECTED_EDGE_COLOR   (sf::Color::Red)
#define NEAREST_EDGE_COLOR  (sf::Color::Magenta)
#define NODE_RADIUS 30

enum EdgeState {
    EDGE_DISCOVERED,
    EDGE_UNDISCOVERED,
    EDGE_NEAREST,
    EDGE_SELECTED
};

class Edge {
    sf::RectangleShape shape;
    std::string literal;
    const std::shared_ptr<Node> first_node;
    const std::shared_ptr<Node> second_node;
    float length;


public:
    explicit Edge(const std::shared_ptr<Node>& first_node, const std::shared_ptr<Node>& second_node, bool do_correct = true);
    ~Edge() = default;
    void render(sf::RenderTarget& target);
    void setColor(const sf::Color& color);
    void setState(EdgeState state);
    int getLength() const;
    bool operator<(const std::shared_ptr<Edge>& other) const;
    bool operator==(const std::shared_ptr<Edge>& other) const;
    std::vector<float> correctEdgeCoordinates();
    std::shared_ptr<Node> getFirstNode() const;
    std::shared_ptr<Node> getSecondNode() const;
    sf::RectangleShape getShape() const;
};

#endif //ALGOVIZ_EDGE_H
