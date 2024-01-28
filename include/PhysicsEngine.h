#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "Particle.h"
#include "Wall.h"
#include <vector>

class PhysicsEngine {
public:
    void addParticle(const Particle& particle);
    void addWall(const Wall& wall);

    void update(float deltaTime); // Update all particles

private:
    std::vector<Particle> particles;
    std::vector<Wall> walls;

    void resolveCollisions(Particle& particle);
    bool checkCollision(const Particle& particle, const Wall& wall) const;
    void handleCollision(Particle& particle, const Wall& wall);
};

#endif // PHYSICS_ENGINE_H