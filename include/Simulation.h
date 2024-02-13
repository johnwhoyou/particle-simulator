#ifndef SIMULATION_H
#define SIMULATION_H

#include "Particle.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>

class Simulation {
private:
    std::vector<Particle> particles;
    std::vector<Wall> walls;

    void resolveCollisions(Particle& particle);
    bool checkCollision(const Particle& particle, const Wall& wall) const;
    void handleCollision(Particle& particle, const Wall& wall);

public:
    void update(double deltaTime);
    void addParticle(int x, int y, double angle, double velocity);
    void addWall(int x1, int y1, int x2, int y2);
    std::vector<Particle> getParticles();

};

#endif // SIMULATION_H