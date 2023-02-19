//
// Created by User on 2/14/2023.
//

#ifndef ALGOVIZ_UTILS_H
#define ALGOVIZ_UTILS_H
#include "SFML/Graphics.hpp"
#include <vector>


float getAngle(float x1, float y1, float x2, float y2);
float getDistance(float x1, float y1, float x2, float y2);
float getCorrectedX1(float x1, float angle);
float getCorrectedY1(float y1, float angle);
std::vector<float> getClosestNonCollision(float node_x, float node_y, float cursor_x, float cursor_y);

#endif //ALGOVIZ_UTILS_H
