//
// Created by User on 12/19/2022.
//
#ifndef ALGOVIZ_EDGE_H
#define ALGOVIZ_EDGE_H

#include "Node.h"
#include "SFML/Graphics.hpp"

#define UNDISCOVERED_EDGE_COLOR (sf::Color(192,192,192,70))
#define DISCOVERED_EDGE_COLOR   (sf::Color::White)

enum edge_state {EDGE_DISCOVERED, EDGE_UNDISCOVERED};

class Edge{
    std::string literal;
    const std::shared_ptr<Node> node1;
    const std::shared_ptr<Node> node2;
    int weight = 0;
    edge_state state = EDGE_UNDISCOVERED;
    sf::RectangleShape shape;

public:
    explicit Edge(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2);
    ~Edge() = default;
    const std::shared_ptr<Node>& getNode1() const;
    const std::shared_ptr<Node>& getNode2() const;
    sf::RectangleShape getShape() const;
    void render(sf::RenderTarget& target);
    void setColor(const sf::Color& color);
    sf::Color getColor() const;
    void setState(edge_state state);
    edge_state getState() const;
    bool operator<(const std::shared_ptr<Edge>& other) const;
    bool operator==(const std::shared_ptr<Edge>& other) const;
};

#endif //ALGOVIZ_EDGE_H
