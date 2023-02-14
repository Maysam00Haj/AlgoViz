//
// Created by User on 2/14/2023.
//
#include <cmath>
#include <numbers>
#include "utils.h"

#ifndef NODE_RADIUS
#define NODE_RADIUS 30
#endif

float getAngle(float x1, float y1, float x2, float y2) {
    float rads = std::atan((y2 - y1) / (x2 - x1));
    float degrees = (rads * 180)/(float)std::numbers::pi;
    float angle = degrees - 90; // correction for shape::setRotation function
    if (x1 > x2) angle += 180; // another correction
    return angle;
}

float getDistance(float x1, float y1, float x2, float y2) {
    auto distance = (float)sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distance;
}

float getCorrectedX1(float x1, float angle) {
    float rads = ((angle + 90) * (float)std::numbers::pi) / 180;
    float offset = NODE_RADIUS * std::cos(rads);
    return x1 + offset;
}

float getCorrectedY1(float y1, float angle) {
    float rads = ((angle+90) * (float)std::numbers::pi) / 180;
    float offset = NODE_RADIUS * std::sin(rads);
    return y1 + offset;
}