#include "Renderer.h"
// #include <GL/gl.h>

Renderer::Renderer(int width, int height)
    : width(width), height(height) {
    // ...
}

Renderer::~Renderer() {
    // Cleanup OpenGL resources
    // ...
}

void Renderer::initialize() {
    // Initialize OpenGL context
    // Setup shaders, buffers, and any other OpenGL state needed
    // ...
}

void Renderer::render(const std::vector<Particle>& particles, const std::vector<Wall>& walls) {
    // Clear the canvas
    //glClear(GL_COLOR_BUFFER_BIT);

    // Render particles
    for (const auto& particle : particles) {
        // Use OpenGL commands to draw the particle
        // ...
    }

    // Render walls
    for (const auto& wall : walls) {
        // Use OpenGL commands to draw the wall
        // ...
    }

    // Swap buffers if using double buffering
    // ...
}

// ... (Any additional OpenGL setup and cleanup)