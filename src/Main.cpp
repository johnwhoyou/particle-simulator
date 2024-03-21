#include "GUI.h"
#include "glad/glad.h"
#include "SDL.h"
#include "SDL_image.h"
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

    SDL_Surface* surface = IMG_Load("sprite.png");
    if (!surface) {
        std::cout << "Incorrect directory" << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cout << "texture failure" << std::endl;
    }

    SDL_FreeSurface(surface);

    Simulation simulation;
    simulation.initializeCanvasBoundaries();
    MainGUI simulatorGUI;
    simulatorGUI.Init(window, renderer, texture);
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
            else if (event.type == SDL_KEYDOWN && simulatorGUI.getExplorerMode()) {
                switch (event.key.keysym.sym) {
                case SDLK_w:
                    simulation.moveSprite(1);
                    break;
                case SDLK_UP:
                    simulation.moveSprite(1);
                    break;
                case SDLK_s:
                    simulation.moveSprite(3);
                    break;
                case SDLK_DOWN:
                    simulation.moveSprite(3);
                    break;
                case SDLK_a:
                    simulation.moveSprite(2);
                    break;
                case SDLK_LEFT:
                    simulation.moveSprite(2);
                    break;
                case SDLK_d:
                    simulation.moveSprite(0);
                    break;
                case SDLK_RIGHT:
                    simulation.moveSprite(0);
                    break;
                default:
                    break;
                }
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
    };

    simulatorGUI.Shutdown();
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
