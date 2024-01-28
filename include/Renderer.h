#ifndef RENDERER_H
#define RENDERER_H

#include "Particle.h"
#include "Wall.h"
#include <vector>

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    void initialize(); // Setup OpenGL context
    void render(const std::vector<Particle>& particles, const std::vector<Wall>& walls);

private:
    int width, height; // Size of the canvas

    // OpenGL context variables (buffers, shaders, etc.)
    // ...
};

#endif // RENDERER_H