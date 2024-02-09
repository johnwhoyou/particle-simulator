#include "Wall.h"
#define _USE_MATH_DEFINES
#include <cmath>

Wall::Wall(float x1, float y1, float x2, float y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {}

float Wall::getX1() const {
    return x1;
}

float Wall::getY1() const {
    return y1;
}

float Wall::getX2() const {
    return x2;
}

float Wall::getY2() const {
    return y2;
}

float Wall::getAngle() const {
    // Calculate the angle of the wall in degrees
    return std::atan2(y2 - y1, x2 - x1) * 180.0f / M_PI;
}

// ... (Any additional implementation details)