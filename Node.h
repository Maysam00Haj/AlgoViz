#ifndef ALGOVIZ_NODE_H
#define ALGOVIZ_NODE_H

#include "SFML/Graphics.hpp"
#include <string>
#include <memory>

#define START_NODE_COLOR        (sf::Color::Cyan)
#define CURRENT_NODE_COLOR      (sf::Color::Yellow)
#define UNDISCOVERED_NODE_COLOR (sf::Color::White)
#define DISCOVERED_NODE_COLOR   (sf::Color::Red)
#define DONE_NODE_COLOR         (sf::Color::Blue)


enum NodeState {NODE_CURRENT, NODE_START, NODE_UNDISCOVERED, NODE_DISCOVERED, NODE_DONE};

class Node {
    std::string name;
    NodeState state = NODE_UNDISCOVERED;
    int distance = INT_MAX;
    int path_weight = INT_MAX;
    sf::CircleShape shape;
    bool is_toggled = false;

public:
    explicit Node(std::string name, float pos_x, float pos_y);
    sf::CircleShape getShape() const;
    sf::Rect<float> getShapeLocalBounds() const;
    void render(sf::RenderTarget& target);
    std::string getName() const;
    bool checkBoundsCollision(const std::shared_ptr<Node>& node) const;
    void setDistance(int distance);
    int getDistance() const;
    void setPathWeight(int path_weight);
    int getPathWeight() const;
    bool operator==(const std::shared_ptr<Node>& node) const;
    void setColor(const sf::Color& color);
    sf::Color getColor() const;
    void setState(NodeState state);
    NodeState getState() const;
    sf::Vector2f getPosition() const;
    void setPosition(float pos_x, float pos_y);
    void toggle();
    void untoggle();

};



#endif //ALGOVIZ_NODE_H
