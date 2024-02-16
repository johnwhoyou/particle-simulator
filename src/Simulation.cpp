#include "Simulation.h"

void Simulation::update(double deltaTime) {
    for (auto& particle : particles) {
        particle.update(deltaTime);
        resolveCollisions(particle, deltaTime);
    }
}

void Simulation::initializeCanvasBoundaries() {
    addWall(0, 0, 0, 720);
    addWall(0, 720, 1280, 720);
    addWall(1280, 0, 1280, 720);
    addWall(0, 0, 1280, 0);
}

void Simulation::addWall(int x1, int y1, int x2, int y2) {
    walls.emplace_back(x1, y1, x2, y2);
}

void Simulation::addParticle(double x, double y, double angle, double velocity) {
    particles.emplace_back(x, y, angle, velocity);
}

void Simulation::addParticleByBatchMethod1(double x1, double y1, double x2, double y2, double angle, double velocity, int n) {
    if (n <= 1) {
        addParticle(x1, y1, angle, velocity);
    }
    else {
        double dx = x2 - x1;
        double dy = y2 - y1;

        double stepX = dx / (n - 1);
        double stepY = dy / (n - 1);

        for (int i = 0; i < n; ++i) {
            double newX = x1 + stepX * i;
            double newY = y1 + stepY * i;
            addParticle(newX, newY, angle, velocity);
        }
    }
}

void Simulation::addParticleByBatchMethod2(double x, double y, double startAngle, double endAngle, double velocity, int n) {
    if (n <= 1) {
        addParticle(x, y, startAngle, velocity);
    }
    else {
        double angleStep = (endAngle - startAngle) / (n - 1);

        for (int i = 0; i < n; ++i) {
            double angle = startAngle + angleStep * i;
            addParticle(x, y, angle, velocity);
        }
    }
}

void Simulation::addParticleByBatchMethod3(double x, double y, double angle, double startVelocity, double endVelocity, int n) {
    if (n <= 1) {
        addParticle(x, y, angle, startVelocity);
    }
    else {
        double velocityStep = (endVelocity - startVelocity) / (n - 1);

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
    if (walls.size() > 4) {
        walls.erase(walls.begin() + 4, walls.end());
    }
}

void Simulation::clearAll() {
    clearParticles();
    clearWalls();
}

std::vector<Particle> Simulation::getParticles() {
    return particles;
}

std::vector<Wall> Simulation::getWalls() {
    return walls;
}

void Simulation::resolveCollisions(Particle& particle, double deltaTime) {
    for (const auto& wall : walls) {
        if (checkCollision(particle, wall, deltaTime)) {
            handleCollision(particle, wall);
        }
    }
}

bool Simulation::checkCollision(const Particle& particle, const Wall& wall, double deltaTime) const {
    double radians = particle.getAngle() * M_PI / 180.0;
    double radius = 5.0;

    double predictedX = particle.getX() + std::cos(radians) * particle.getVelocity() * deltaTime;
    double predictedY = particle.getY() + std::sin(radians) * particle.getVelocity() * deltaTime;

    double wallStartX = wall.getX1();
    double wallStartY = wall.getY1();
    double wallEndX = wall.getX2();
    double wallEndY = wall.getY2();

    double wallVecX = wallEndX - wallStartX;
    double wallVecY = wallEndY - wallStartY;

    double startToCircleX = predictedX - wallStartX;
    double startToCircleY = predictedY - wallStartY;

    double wallLengthSquared = wallVecX * wallVecX + wallVecY * wallVecY;
    double dotProduct = startToCircleX * wallVecX + startToCircleY * wallVecY;
    double projection = dotProduct / wallLengthSquared;

    projection = std::max(0.0, std::min(1.0, projection));

    double closestX = wallStartX + projection * wallVecX;
    double closestY = wallStartY + projection * wallVecY;

    double distanceX = closestX - predictedX;
    double distanceY = closestY - predictedY;
    double distanceSquared = distanceX * distanceX + distanceY * distanceY;

    return distanceSquared <= (radius * radius);
}

void Simulation::handleCollision(Particle& particle, const Wall& wall) {
    particle.bounceOffWall(wall);
}
