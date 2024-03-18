#include "Sprite.h"
#include <string>

Sprite::Sprite(double x, double y)
    : x(x), y(y) {}

//Check which movement or button pressed.
void Sprite::update(string direction) {
    switch (direction) {
    case "UP":
        moveUp(); break;
    case "DOWN":
        moveDown(); break;
    case "LEFT":
        moveLeft(); break;
    case "RIGHT":
        moveRight(); break;
    default: break;
    }
}

void Sprite::moveUp() {
    // x += deltaTime;
    y += 1;
}

void Sprite::moveDown() {
    // x += deltaTime;
    y -= 1;
}

void Sprite::moveLeft() {
    x -= 1;
    //y += 1;
}

void Sprite::moveRight() {
    x += 1;
    // y += 1;
}

double Sprite::getX() const {
    return x;
}

double Sprite::getY() const {
    return y;
}