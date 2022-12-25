//
// Created by User on 12/19/2022.
//
#ifndef ALGOVIZ_EDGE_H
#define ALGOVIZ_EDGE_H

#include "Node.h"

enum edge_color {grey_edge, white_edge};

class Edge{
    Node node1;
    Node node2;
    edge_color color = grey_edge;

public:
    explicit Edge(const Node& node1, const Node& node2);
    ~Edge() = default;
    Edge(const Edge& other);
    Edge& operator=(const Edge& other);
};

#endif //ALGOVIZ_EDGE_H
