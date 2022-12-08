#ifndef ALGOVIZ_NODE_H
#define ALGOVIZ_NODE_H

#include "SFML/Graphics.hpp"
using namespace std;

class Node {
public:
    Node(char name);
    void setShape(float pos_x, float pos_y);
    sf::CircleShape getShape();

private:
    char name;
    bool visited;
    vector<Node*> neighbors;
    sf::CircleShape shape;
};



#endif //ALGOVIZ_NODE_H
