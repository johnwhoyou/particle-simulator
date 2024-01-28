#include "Simulation.h"

Simulation::Simulation() : renderer(1280, 720) {
    running = false;
}

Simulation::~Simulation() {
    stop(); // Ensure threads are joined before destruction
}

void Simulation::start() {
    running = true;
    updateThread = std::thread(&Simulation::updateLoop, this);
    renderThread = std::thread(&Simulation::renderLoop, this);
}

void Simulation::stop() {
    running = false;
    if (updateThread.joinable()) updateThread.join();
    if (renderThread.joinable()) renderThread.join();
}

void Simulation::addParticle(float x, float y, float angle, float velocity) {
    // Lock the mutex to ensure thread safety
    std::lock_guard<std::mutex> guard(particlesMutex);
    particles.emplace_back(x, y, angle, velocity);
}

void Simulation::addWall(float x1, float y1, float x2, float y2) {
    // Lock the mutex to ensure thread safety
    std::lock_guard<std::mutex> guard(wallsMutex);
    walls.emplace_back(x1, y1, x2, y2);
}

void Simulation::updateLoop() {
    while (running) {
        // Calculate delta time
        // ...

        // Update physics
        physicsEngine.update(deltaTime);

        // ... (Any additional update logic)
    }
}

void Simulation::renderLoop() {
    renderer.initialize();
    while (running) {
        // Lock the mutexes to access shared resources
        {
            std::lock_guard<std::mutex> particlesGuard(particlesMutex);
            std::lock_guard<std::mutex> wallsGuard(wallsMutex);
            
            renderer.render(particles, walls);
        }

        // Display FPS counter
        // ...

        // ... (Any additional rendering logic)
    }
}

// ... (Any additional implementation details)