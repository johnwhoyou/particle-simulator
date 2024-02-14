#ifndef SIMULATION_H
#define SIMULATION_H
#define _USE_MATH_DEFINES

#include "Particle.h"
#include "ThreadPool.h"
#include "ctpl_stl.h"
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
    ctpl::thread_pool threadPool;

    void resolveCollisions(Particle& particle);
    bool checkCollision(const Particle& particle, const Wall& wall) const;
    void handleCollision(Particle& particle, const Wall& wall);
    void updateParticlesInRange(size_t startIdx, size_t endIdx, double deltaTime);

public:
    Simulation(int nThreads = std::thread::hardware_concurrency()) : threadPool(nThreads) {}
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

#endif // SIMULATION_H