#ifndef ALGOVIZ_NODE_H
#define ALGOVIZ_NODE_H

#include "SFML/Graphics.hpp"
#include <string>
#include <memory>


class Node {
    enum node_state {CURRENT, UNDISCOVERED, DISCOVERED, DONE};

    std::string name;
    node_state state = UNDISCOVERED;
    int distance = INT_MAX;
    int path_weight = INT_MAX;
    std::vector<std::shared_ptr<Node>> neighbors;
    sf::CircleShape shape;

public:
    explicit Node(std::string name, float pos_x, float pos_y);
    sf::CircleShape getShape() const;
    sf::Rect<float> getShapeLocalBounds() const;
    void render(sf::RenderTarget& target);
    std::string getName() const;
    bool checkCollision(const std::shared_ptr<Node>& node) const;
    void setDistance(int distance);
    int getDistance() const;
    void setPathWeight(int path_weight);
    int getPathWeight() const;
    bool operator==(const Node& node) const;
    void setColor(const sf::Color& color);
    sf::Color getColor() const;
    void setState(node_state state);
    node_state getState() const;

};



#endif //ALGOVIZ_NODE_H
