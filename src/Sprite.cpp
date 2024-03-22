#include "Sprite.h"
#include <string>

Sprite::Sprite(double x, double y)
    : x(x), y(y) {}

void Sprite::moveUp() {
    if (y - 20 > 0) {
        y -= 20;
    }
    else {
        y = 0;
    }
    
}

void Sprite::moveDown() {
    if (y + 20 < 720) {
        y += 20;
    }
    else {
        y = 720;
    }
}

void Sprite::moveLeft() {
    if (x - 20 > 0) {
        x -= 20;
    }
    else {
        x = 0;
    }
}

void Sprite::moveRight() {
    if (x + 20 < 1280) {
        x += 20;
    }
    else {
        x = 1280;
    }
    
}

void Sprite::reset() {
	x = 640;
	y = 360;
}

double Sprite::getX() const {
    return x;
}

double Sprite::getY() const {
    return y;
}