#include "GUI.h"
#include "glad/glad.h"
#include "SDL.h"
#undef main

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return 1;

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow("STDISCM Particle Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    if (window == NULL)
        return 1;

    SDL_SetWindowResizable(window, SDL_FALSE);

    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
        return 1;

    // Initialize GLAD
    if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
        return 1;

    SDL_GL_SetSwapInterval(1); // Enable vsync

    Simulation simulation;
    simulation.initializeCanvasBoundaries();
    MainGUI simulatorGUI;
    simulatorGUI.Init(window, "#version 130");
    simulatorGUI.setSimulation(&simulation);

    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    bool isRunning = true;
    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                isRunning = false;

            ImGui_ImplSDL2_ProcessEvent(&event);
        }
        
        // Calculate delta time
        auto now = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastFrameTime).count();
        lastFrameTime = now;

        // Update the simulation with the calculated delta time
        simulation.update(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT);
        simulatorGUI.NewFrame(window);
        simulatorGUI.Update();
        simulatorGUI.Render();
        SDL_GL_SwapWindow(window);
    }

    simulatorGUI.Shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
