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
    sf::Vertex line[2];

public:
    explicit Edge(const Node& node1, const Node& node2);
    ~Edge() = default;
    Node getNode1() const;
    Node getNode2() const;
    void render(sf::RenderTarget& target);
    bool operator<(const Edge& other) const;
    bool operator==(const Edge& other) const;
};

#endif //ALGOVIZ_EDGE_H
