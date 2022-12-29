#ifndef ALGOVIZ_NODE_H
#define ALGOVIZ_NODE_H

#include "SFML/Graphics.hpp"
#include <string>
#include <memory>

enum node_state {current, undiscovered, discovered, done};
enum node_color {yellow, white, red, blue};

class Node {
    std::string name;
    node_state state = undiscovered;
    node_color color = white;
    int distance = INT_MAX;
    int path_weight = INT_MAX;
    std::vector<std::shared_ptr<Node>> neighbors;
    sf::CircleShape shape;

public:
    explicit Node(std::string name);
    void setShape(float pos_x, float pos_y);
    sf::CircleShape getShape();
    std::string getName() const;
    void setDistance(int distance);
    int getDistance() const;
    void setPathWeight(int path_weight);
    int getPathWeight() const;
    bool operator==(const Node& node) const;
};



#endif //ALGOVIZ_NODE_H
