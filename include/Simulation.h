#ifndef SIMULATION_H
#define SIMULATION_H
#define _USE_MATH_DEFINES

#include "Particle.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <optional>
#include <tuple>
#include <cmath>

class Simulation {
private:
    std::vector<Particle> particles;
    std::vector<Wall> walls;

    void resolveCollisions(Particle& particle, double deltaTime);
    bool checkCollision(const Particle& particle, const Wall& wall, double deltaTime) const;
    void handleCollision(Particle& particle, const Wall& wall);

public:
    void update(double deltaTime);
    void initializeCanvasBoundaries();
    void addWall(int x1, int y1, int x2, int y2);
    void addParticle(double x, double y, double angle, double velocity);
    void addParticleByBatchMethod1(double x1, double y1, double x2, double y2, double angle, double velocity, int n);
    void addParticleByBatchMethod2(double x, double y, double startAngle, double endAngle, double velocity, int n);
    void addParticleByBatchMethod3(double x, double y, double angle, double startVelocity, double endVelocity, int n);
    void clearParticles();
    void clearWalls();
    void clearAll();
    std::vector<Particle> getParticles();
    std::vector<Wall> getWalls();

};

#endif