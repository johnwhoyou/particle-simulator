#ifndef SIMULATION_H
#define SIMULATION_H

#include "PhysicsEngine.h"
#include "Renderer.h"
#include <thread>
#include <atomic>
#include <mutex>

class Simulation {
public:
    Simulation();
    ~Simulation();

    void start(); // Start the simulation loop
    void stop();  // Signal the simulation to stop

    // Methods to pass input from GUI to simulation
    void addParticle(float x, float y, float angle, float velocity);
    void addWall(float x1, float y1, float x2, float y2);

private:
    PhysicsEngine physicsEngine;
    Renderer renderer;

    std::thread updateThread;   // Thread for running the physics update loop
    std::thread renderThread;   // Thread for running the rendering loop

    std::atomic<bool> running;  // Flag to control the simulation loop

    std::mutex particlesMutex;  // Mutex to protect shared particle data
    std::mutex wallsMutex;      // Mutex to protect shared wall data

    void updateLoop(); // Physics update loop
    void renderLoop(); // Rendering loop

    // Shared resources between threads
    std::vector<Particle> particles;
    std::vector<Wall> walls;

    // ... (Any additional thread management and synchronization logic)
};

#endif // SIMULATION_H