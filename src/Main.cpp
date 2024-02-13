#include "GUI.h"
#include "glad/glad.h"
#include "SDL.h"
#undef main

void drawCircle(SDL_Renderer* renderer, int32_t centerX, int32_t centerY, int32_t radius) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowResizable(window, SDL_FALSE);

    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
        return 1;

    // Initialize GLAD
    if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
        return 1;

    SDL_GL_SetSwapInterval(1); // Enable vsync

    MainGUI simulatorGUI;
    simulatorGUI.Init(window, "#version 130");

    bool isRunning = true;
    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                isRunning = false;

            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        simulatorGUI.NewFrame(window);
        simulatorGUI.Update();
        simulatorGUI.Render();

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw the red circle
        drawCircle(renderer, 640, 360, 100); // Drawing at the center of the 1280x720 canvas

        // Present the renderer
        SDL_RenderPresent(renderer);

        SDL_GL_SwapWindow(window);
    }

    simulatorGUI.Shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
