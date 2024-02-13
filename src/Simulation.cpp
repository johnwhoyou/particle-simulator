#include "Simulation.h"

void Simulation::update(double deltaTime) {
    for (auto& particle : particles) {
        particle.update(deltaTime);
        resolveCollisions(particle);
    }
}

void Simulation::addWall(int x1, int y1, int x2, int y2) {
    walls.emplace_back(x1, y1, x2, y2);
}

void Simulation::addParticle(double x, double y, double angle, double velocity) {
    particles.emplace_back(x, y, angle, velocity);
}

void Simulation::addParticleByBatchMethod1(double x1, double y1, double x2, double y2, double angle, double velocity, int n) {
    if (n <= 1) {
        // If only one particle is to be added, add it at the start point
        addParticle(x1, y1, angle, velocity);
    }
    else {
        // Calculate the vector from the start to the end point
        double dx = x2 - x1;
        double dy = y2 - y1;

        // Calculate the distance between particles
        double stepX = dx / (n - 1);
        double stepY = dy / (n - 1);

        // Add particles along the line
        for (int i = 0; i < n; ++i) {
            double newX = x1 + stepX * i;
            double newY = y1 + stepY * i;
            addParticle(newX, newY, angle, velocity);
        }
    }
}

void Simulation::addParticleByBatchMethod2(double x, double y, double startAngle, double endAngle, double velocity, int n) {
    if (n <= 1) {
        // If only one particle is to be added, add it at the start point with the startAngle
        addParticle(x, y, startAngle, velocity);
    }
    else {
        // Calculate the angle step for each particle
        double angleStep = (endAngle - startAngle) / (n - 1);

        // Add particles with angles evenly distributed between startAngle and endAngle
        for (int i = 0; i < n; ++i) {
            double angle = startAngle + angleStep * i;
            addParticle(x, y, angle, velocity);
        }
    }
}

void Simulation::addParticleByBatchMethod3(double x, double y, double angle, double startVelocity, double endVelocity, int n) {
    if (n <= 1) {
        // If only one particle is to be added, add it with the startVelocity
        addParticle(x, y, angle, startVelocity);
    }
    else {
        // Calculate the velocity step for each particle
        double velocityStep = (endVelocity - startVelocity) / (n - 1);

        // Add particles with velocities evenly distributed between startVelocity and endVelocity
        for (int i = 0; i < n; ++i) {
            double velocity = startVelocity + velocityStep * i;
            addParticle(x, y, angle, velocity);
        }
    }
}

void Simulation::clearParticles() {
    particles.clear();
}

void Simulation::clearWalls() {
    walls.clear();
}

void Simulation::clearParticles() {
    clearParticles();
    clearWalls();
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
    double radius = 5.0;

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
