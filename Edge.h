//
// Created by User on 12/19/2022.
//
#ifndef ALGOVIZ_EDGE_H
#define ALGOVIZ_EDGE_H

#include "Node.h"
#include "SFML/Graphics.hpp"

enum edge_color {grey_edge, white_edge};

class Edge{
    std::string literal;
    const Node node1;
    const Node node2;
    int weight = 0;
    edge_color color = grey_edge;
    sf::RectangleShape shape;

public:
    explicit Edge(Node node1, Node
    node2);
    ~Edge() = default;
    Node getNode1() const;
    Node getNode2() const;
    void setShape(float pos_x1, float pos_y1, float pos_x2, float pos_y2);
    sf::RectangleShape getShape() const;
};

#endif //ALGOVIZ_EDGE_H
