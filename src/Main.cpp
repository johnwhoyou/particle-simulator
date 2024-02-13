#include "GUI.h"
#include "glad/glad.h"
#include "SDL.h"
#undef main

void drawCircleOpenGL(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    for(int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments); // Current angle
        float x = r * cosf(theta); // Calculate the x component
        float y = r * sinf(theta); // Calculate the y component
        glVertex2f(x + cx, y + cy); // Output vertex
    }
    glEnd();
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

        // Make sure to set up an appropriate OpenGL projection matrix if needed
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the circle
        drawCircleOpenGL(640.0f, 360.0f, 100.0f, 32); // Example: Draw a circle at the center

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
