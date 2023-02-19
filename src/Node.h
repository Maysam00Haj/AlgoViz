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
#define NODE_OUTLINE_COLOR      (sf::Color::Yellow)


enum NodeState {
    NODE_CURRENT,
    NODE_START,
    NODE_UNDISCOVERED,
    NODE_DISCOVERED,
    NODE_DONE
};

class Node {
private:
    std::string name;
    NodeState state = NODE_UNDISCOVERED;
    sf::CircleShape shape;
    int distance = INT_MAX;

public:
    explicit Node(std::string name, float pos_x, float pos_y, sf::Font* text_font);
    ~Node() = default;
    void render(sf::RenderTarget& target);
    bool checkBoundsCollision(const std::shared_ptr<Node>& node) const;
    void setDistance(int distance);
    int getDistance() const;
    void setPathWeight(int path_weight);
    bool operator==(const std::shared_ptr<Node>& node) const;
    void setColor(const sf::Color& color);
    void setState(NodeState state);
    void setPosition(float pos_x, float pos_y);
    void toggle();
    void untoggle();
    sf::CircleShape getShape() const;
    std::string getName() const;
    NodeState getState() const;
    sf::Vector2f getPosition() const;
};



#endif //ALGOVIZ_NODE_H
