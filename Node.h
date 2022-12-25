#ifndef ALGOVIZ_NODE_H
#define ALGOVIZ_NODE_H

#include "SFML/Graphics.hpp"
#include <string>
using namespace std;

enum node_state {current, undiscovered, discovered, done};
enum node_color {yellow, white, red, blue};

class Node {
    string name;
    node_state state = undiscovered;
    node_color color = white;
    vector<Node*> neighbors;
    sf::CircleShape shape;

public:
    explicit Node(string name);
    void setShape(float pos_x, float pos_y);
    sf::CircleShape getShape();
};



#endif //ALGOVIZ_NODE_H
