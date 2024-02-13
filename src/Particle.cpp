#include "Particle.h"
#define _USE_MATH_DEFINES
#include <cmath>

constexpr double degToRad(double degrees) {
    return degrees * M_PI / 180.0f;
}

// Function to Normalize the angle to be within [0, 360)
double normalizeAngle(double angle) {
    return fmod(fmod(angle, 360.0f) + 360.0f, 360.0f);
}

Particle::Particle(int x, int y, double angle, double velocity)
    : x(x), y(y), angle(normalizeAngle(angle)), velocity(velocity) {}

void Particle::update(double deltaTime) {
    move(deltaTime);
}

void Particle::move(double deltaTime) {
    // Calculate movement based on velocity and angle
    x += std::cos(degToRad(angle)) * velocity * deltaTime;
    y += std::sin(degToRad(angle)) * velocity * deltaTime;
}

void Particle::bounceOffWall(const Wall& wall) {
    double wallAngle = wall.getAngle();  // angle of the wall with respect to the positive x-axis

    // check if the wall is vertical (east or west wall)
    if (wallAngle == 0.0f || wallAngle == 180.0f) {
        // if vertical wall, simply mirror the horizontal component of the angle
        angle = 360.0f - angle;
    } else if (wallAngle == 90.0f || wallAngle == 270.0f) {
        // if horizontal wall (north or south wall), mirror the vertical component
        angle = 180.0f - angle;
    } else {
        // for walls with arbitrary angles, use the previous reflection logic
        double normalAngle;
        if (isParticleOnRightSideOfWall(wall)) {
            normalAngle = normalizeAngle(wallAngle + 90.0f);
        } else {
            normalAngle = normalizeAngle(wallAngle - 90.0f);
        }
        double angleOfIncidence = normalizeAngle(angle - normalAngle);
        angle = normalizeAngle(normalAngle - angleOfIncidence);
    }

    // normalize the final angle
    angle = normalizeAngle(angle);
}

bool Particle::isParticleOnRightSideOfWall(const Wall& wall) const {
    // use the cross product to determine the relative position
    // compute wall vector
    double wallVecX = wall.getX2() - wall.getX1();
    double wallVecY = wall.getY2() - wall.getY1();
    // compute particle vector
    int particleVecX = x - wall.getX1();
    int particleVecY = y - wall.getY1();
    // cross product to determine the side
    int crossProduct = (wallVecX * particleVecY) - (wallVecY * particleVecX);
    
    return crossProduct > 0; // if cross product is positive, particle is on right side
}

int Particle::getX() const {
    return x;
}

int Particle::getY() const {
    return y;
}

double Particle::getAngle() const {
    return angle;
}