#ifndef SIMULATION_H
#define SIMULATION_H
#define _USE_MATH_DEFINES

#include "Particle.h"
#include "Sprite.h"
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
    std::vector<Sprite> serverSprites;
    std::vector<Sprite> clientSprites;
    std::vector<std::pair<double, double>> serverParticles;
    std::vector<std::pair<double, double>> clientParticles;
    std::vector<Particle> particles;
    std::vector<Wall> walls;
    ctpl::thread_pool threadPool;
    Sprite sprite;

    void resolveCollisions(Particle& particle, double deltaTime);
    bool checkCollision(const Particle& particle, const Wall& wall, double deltaTime) const;
    void handleCollision(Particle& particle, const Wall& wall);
    void updateParticlesInRange(size_t startIdx, size_t endIdx, double deltaTime);

public:
    Simulation(int nThreads = std::thread::hardware_concurrency(), double x = 640, double y = 360) : threadPool(nThreads), sprite(x, y) {}
    void update(double deltaTime);
    void moveSprite(char direction);
    void initializeCanvasBoundaries();
    void addWall(int x1, int y1, int x2, int y2);
    void addParticle(double x, double y, double angle, double velocity);
    void addParticleByBatchMethod1(double x1, double y1, double x2, double y2, double angle, double velocity, int n);
    void addParticleByBatchMethod2(double x, double y, double startAngle, double endAngle, double velocity, int n);
    void addParticleByBatchMethod3(double x, double y, double angle, double startVelocity, double endVelocity, int n);
    void clearParticles();
    void resetSprite();
    std::vector<Particle> getParticles();
    std::vector<Wall> getWalls();
    Sprite getSprite();

    void addServerParticle(double x, double y);
    void setServerParticles(const std::vector<std::pair<double, double>>& particleList);
    void updateClientParticles();
    std::vector<std::pair<double, double>> getClientParticles();
    std::vector<std::pair<double, double>> getServerParticles();
    void clearClientParticles();

    std::vector<Sprite> getClientSprites();
    void setServerSprites(const std::vector<Sprite>& spriteList);
    void updateClientSprites();
    void updateClient();
};

#endif