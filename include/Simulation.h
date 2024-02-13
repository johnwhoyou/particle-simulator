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
    void addWall(int x1, int y1, int x2, int y2);
    void addParticle(int x, int y, double angle, double velocity);
    void addParticleByBatchMethod1(int x1, int y1, int x2, int y2, double angle, double velocity, int n);
    void addParticleByBatchMethod2(int x, int y, double startAngle, double endAngle, double velocity, int n);
    void addParticleByBatchMethod3(int x, int y, double angle, double startVelocity, double endVelocity, int n);
    std::vector<Particle> getParticles();

};

#endif // SIMULATION_H