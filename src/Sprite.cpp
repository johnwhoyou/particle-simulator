#include "Sprite.h"
#include <string>

Sprite::Sprite(double x, double y)
    : x(x), y(y) {}

void Sprite::moveUp() {
    y -= 20;
}

void Sprite::moveDown() {
    y += 20;
}

void Sprite::moveLeft() {
    x -= 20;
}

void Sprite::moveRight() {
    x += 20;
}

double Sprite::getX() const {
    return x;
}

double Sprite::getY() const {
    return y;
}