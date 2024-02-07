#include "Particle.h"
#include <cmath>

constexpr float degToRad(float degrees) {
    return degrees * M_PI / 180.0f;
}

// Function to Normalize the angle to be within [0, 360)
float normalizeAngle(float angle) {
    return fmod(fmod(angle, 360.0f) + 360.0f, 360.0f);
}

Particle::Particle(float x, float y, float angle, float velocity)
    : x(x), y(y), angle(normalizeAngle(angle)), velocity(velocity) {}

void Particle::update(float deltaTime) {
    move(deltaTime);
}

void Particle::move(float deltaTime) {
    // Calculate movement based on velocity and angle
    x += std::cos(degToRad(angle)) * velocity * deltaTime;
    y += std::sin(degToRad(angle)) * velocity * deltaTime;
}

void Particle::bounceOffWall(const Wall& wall) {
    float wallAngle = wall.getAngle();  // angle of the wall with respect to the positive x-axis

    // check if the wall is vertical (east or west wall)
    if (wallAngle == 0.0f || wallAngle == 180.0f) {
        // if vertical wall, simply mirror the horizontal component of the angle
        angle = 360.0f - angle;
    } else if (wallAngle == 90.0f || wallAngle == 270.0f) {
        // if horizontal wall (north or south wall), mirror the vertical component
        angle = 180.0f - angle;
    } else {
        // for walls with arbitrary angles, use the previous reflection logic
        float normalAngle;
        if (isParticleOnRightSideOfWall(wall)) {
            normalAngle = normalizeAngle(wallAngle + 90.0f);
        } else {
            normalAngle = normalizeAngle(wallAngle - 90.0f);
        }
        float angleOfIncidence = normalizeAngle(angle - normalAngle);
        angle = normalizeAngle(normalAngle - angleOfIncidence);
    }

    // normalize the final angle
    angle = normalizeAngle(angle);
}

bool Particle::isParticleOnRightSideOfWall(const Wall& wall) const {
    // use the cross product to determine the relative position
    // compute wall vector
    float wallVecX = wall.getX2() - wall.getX1();
    float wallVecY = wall.getY2() - wall.getY1();
    // compute particle vector
    float particleVecX = x - wall.getX1();
    float particleVecY = y - wall.getY1();
    // cross product to determine the side
    float crossProduct = (wallVecX * particleVecY) - (wallVecY * particleVecX);
    
    return crossProduct > 0; // if cross product is positive, particle is on right side
}

float Particle::getX() const {
    return x;
}

float Particle::getY() const {
    return y;
}

float Particle::getAngle() const {
    return angle;
}