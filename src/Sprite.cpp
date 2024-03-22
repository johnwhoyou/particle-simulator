#include "Sprite.h"
#include <string>

Sprite::Sprite(double x, double y)
    : x(x), y(y) {}

void Sprite::moveUp() {
    if (y + STEP < 720) {
        y += STEP;
    }
    else {
        y = 720;
    }
    
}

void Sprite::moveDown() {
    if (y - STEP > 0) {
        y -= STEP;
    }
    else {
        y = 0;
    }
}

void Sprite::moveLeft() {
    if (x - STEP > 0) {
        x -= STEP;
    }
    else {
        x = 0;
    }
}

void Sprite::moveRight() {
    if (x + STEP < 1280) {
        x += STEP;
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