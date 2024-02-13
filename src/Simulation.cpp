#include "Simulation.h"

void Simulation::update(double deltaTime) {
    for (auto& particle : particles) {
        particle.update(deltaTime);
        resolveCollisions(particle);
    }
}

void Simulation::addParticle(int x, int y, double angle, double velocity) {
    particles.emplace_back(x, y, angle, velocity);
}

void Simulation::addWall(int x1, int y1, int x2, int y2) {
    walls.emplace_back(x1, y1, x2, y2);
}

std::vector<Particle> Simulation::getParticles() {
    return particles;
}

void Simulation::resolveCollisions(Particle& particle) {
    for (const auto& wall : walls) {
        if (checkCollision(particle, wall)) {
            handleCollision(particle, wall);
        }
    }
}

bool Simulation::checkCollision(const Particle& particle, const Wall& wall) const {
    // Fixed radius for all particles
    double radius = 10.0;

    // Extract the circle (particle) center
    double circleX = particle.getX();
    double circleY = particle.getY();

    // Extract line segment (wall) endpoints
    double wallStartX = wall.getX1();
    double wallStartY = wall.getY1();
    double wallEndX = wall.getX2();
    double wallEndY = wall.getY2();

    // Calculate the wall's vector
    double wallVecX = wallEndX - wallStartX;
    double wallVecY = wallEndY - wallStartY;

    // Calculate the vector from the wall start to the circle center
    double startToCircleX = circleX - wallStartX;
    double startToCircleY = circleY - wallStartY;

    // Project the startToCircle vector onto the wall vector to find the closest point
    double wallLengthSquared = wallVecX * wallVecX + wallVecY * wallVecY;
    double dotProduct = startToCircleX * wallVecX + startToCircleY * wallVecY;
    double projection = dotProduct / wallLengthSquared;

    // Clamp the projection to the range [0, 1] to stay within the wall segment
    projection = std::max(0.0, std::min(1.0, projection));

    // Calculate the closest point's coordinates
    double closestX = wallStartX + projection * wallVecX;
    double closestY = wallStartY + projection * wallVecY;

    // Calculate the distance from the circle center to the closest point
    double distanceX = closestX - circleX;
    double distanceY = closestY - circleY;
    double distanceSquared = distanceX * distanceX + distanceY * distanceY;

    // Check if the distance is less than the radius squared
    return distanceSquared <= (radius * radius);
}

void Simulation::handleCollision(Particle& particle, const Wall& wall) {
    particle.bounceOffWall(wall);
}
