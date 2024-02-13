#ifndef SIMULATION_H
#define SIMULATION_H

#include "PhysicsEngine.h"
#include "Renderer.h"
#include <thread>
#include <atomic>
#include <mutex>

class Simulation {
private:
    std::vector<Particle> particles;
    std::vector<Wall> walls;

public:
    void addParticle(int x, int y, double angle, double velocity);
    void addWall(int x1, int y1, int x2, int y2);
    std::vector<Particle> getParticles();
};

#endif // SIMULATION_H