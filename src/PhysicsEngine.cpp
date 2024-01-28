#include "PhysicsEngine.h"

void PhysicsEngine::addParticle(const Particle& particle) {
    particles.push_back(particle);
}

void PhysicsEngine::addWall(const Wall& wall) {
    walls.push_back(wall);
}

void PhysicsEngine::update(float deltaTime) {
    for (auto& particle : particles) {
        particle.update(deltaTime);
        resolveCollisions(particle);
    }
}

void PhysicsEngine::resolveCollisions(Particle& particle) {
    for (const auto& wall : walls) {
        if (checkCollision(particle, wall)) {
            handleCollision(particle, wall);
        }
    }
}

bool PhysicsEngine::checkCollision(const Particle& particle, const Wall& wall) const {
    // Implement collision detection logic
    // This will involve checking if the particle's next position intersects with the wall
    // ...
}

void PhysicsEngine::handleCollision(Particle& particle, const Wall& wall) {
    // Implement collision response logic
    // This will involve calculating the new angle of the particle after bouncing off the wall
    particle.bounceOffWall(wall.getAngle());
}

// ... (Any additional implementation details)