#ifndef SPRITE_H
#define SPRITE_H
#define _USE_MATH_DEFINES

#include <cmath>
#include <string>

class Sprite {
public:
    Sprite(double x, double y);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void reset();

    double getX() const;
    double getY() const;
    
private:
    double x, y;
    double STEP = 5;
};

#endif