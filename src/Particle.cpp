#include "../include/Particle.h"
#include <cmath>

Particle::Particle(double x, double y, double angle, double velocity)
    : x(x), y(y), angle(angle), velocity(velocity) {}

void Particle::updatePosition(double timeDelta) {
    // Update the position based on the velocity and angle
    x += velocity * timeDelta * cos(angle * M_PI / 180.0);
    y += velocity * timeDelta * sin(angle * M_PI / 180.0);

    // Handle potential collisions after the position update
    handleCollision();
}

void Particle::handleCollision() {
    // Collision logic goes here
    // This will involve checking the particle's position against the walls and canvas boundaries
    // and adjusting its angle accordingly
}

// Implement other necessary methods
