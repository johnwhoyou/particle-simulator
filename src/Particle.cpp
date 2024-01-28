#include "Particle.h"
#include <cmath>

// Helper function to convert degrees to radians
constexpr float degToRad(float degrees) {
    return degrees * M_PI / 180.0f;
}

Particle::Particle(float x, float y, float angle, float velocity)
    : x(x), y(y), angle(angle), velocity(velocity) {}

void Particle::update(float deltaTime) {
    move(deltaTime);
}

void Particle::move(float deltaTime) {
    // Calculate movement based on velocity and angle
    x += std::cos(degToRad(angle)) * velocity * deltaTime;
    y += std::sin(degToRad(angle)) * velocity * deltaTime;
}

void Particle::bounceOffWall(float wallAngle) {
    // Reflect the movement angle based on the wall's angle
    angle = 2 * wallAngle - angle;
}

float Particle::getX() const {
    return x;
}

float Particle::getY() const {
    return y;
}

// ... (Any additional implementation details)