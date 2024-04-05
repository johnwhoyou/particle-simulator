#include "GUI.h"
#include "glad/glad.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_net.h"
#include "NetworkManager.h"
#undef main

const char* SERVER_ADDRESS = "172.29.118.133";
const int SERVER_TCP_PORT = 3000;
const int SERVER_UDP_PORT = 8000;

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

    std::string spritePath = std::string(PROJECT_ROOT_PATH) + "/res/sprite.png";
    SDL_Surface* surface = IMG_Load(spritePath.c_str());
    if (!surface) {
        std::cout << "Could not load sprite from: " << spritePath << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cout << "texture failure" << std::endl;
    }

    SDL_FreeSurface(surface);

    NetworkManager netManager(SERVER_ADDRESS, SERVER_TCP_PORT, SERVER_UDP_PORT);
    netManager.start();

    Simulation simulation;
    simulation.initializeCanvasBoundaries();
    MainGUI simulatorGUI;
    simulatorGUI.Init(window, renderer, texture);
    simulatorGUI.setNetworkManager(&netManager);
    simulatorGUI.setSimulation(&simulation);
    netManager.setSimulation(&simulation);

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
                const char* command = nullptr;
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                    case SDLK_UP:
                        simulation.moveSprite('U');
                        command = "up";
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        simulation.moveSprite('D');
                        command = "down";
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        simulation.moveSprite('L');
                        command = "left";
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        simulation.moveSprite('R');
                        command = "right";
                        break;
                    default:
                        break;
                }

                if(command) {
                    netManager.sendCommand(command);
                }
            }
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        auto now = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastFrameTime).count();
        lastFrameTime = now;

        //simulation.update(deltaTime);
        simulation.updateClient();
        
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
        // simulation.clearClientParticles();
        SDL_RenderClear(renderer);
        simulatorGUI.GetDrawData();
        SDL_RenderPresent(renderer);
    };

    simulatorGUI.Shutdown();
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    netManager.stop();
    
    return 0;
}
