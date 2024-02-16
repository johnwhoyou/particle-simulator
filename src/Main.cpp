#include "GUI.h"
#include "glad/glad.h"
#include "SDL.h"
#undef main

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "STDISCM Particle Simulator",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1920, 1080,
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_MAXIMIZED
    );

    if (!window) {
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, 
        SDL_RENDERER_ACCELERATED | 
        SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Simulation simulation;
    simulation.initializeCanvasBoundaries();
    MainGUI simulatorGUI;
    simulatorGUI.Init(window, renderer);
    simulatorGUI.setSimulation(&simulation);

    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto startTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    double frameRate = 0.0;

    bool isRunning = true;
    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        auto now = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastFrameTime).count();
        lastFrameTime = now;

        simulation.update(deltaTime);

        auto elapsedSeconds = std::chrono::duration<double>(now - startTime).count();

        if (elapsedSeconds >= 0.5) {
            frameRate = static_cast<double>(frameCount) / elapsedSeconds;
            startTime = now;
            frameCount = 0;
        }
        frameCount++;

        simulatorGUI.NewFrame(window);
        simulatorGUI.Update(frameRate);
        simulatorGUI.Render();
        SDL_RenderClear(renderer);
        simulatorGUI.GetDrawData();
        SDL_RenderPresent(renderer);
    }

    simulatorGUI.Shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
