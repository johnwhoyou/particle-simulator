#pragma once

#include <imgui.h>
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <random>
#include <iostream>

#include "Simulation.h"

class MainGUI {
public:
	void Init(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* spriteTexture);
	void NewFrame(SDL_Window* window);
	void Update(double frameRate);
	void Render();
	void GetDrawData();
	void Shutdown();
	
	bool getExplorerMode();

	void setSimulation(Simulation* sim) {
		simulation = sim;
	}

private:
	void displayCanvas();
	void displayBottomDetails(double frameRate);
	void displayControlsWindow();

	void showAddParticle();
	void showBatchAddParticle();
	void showBatchAddMethod1();
	void showBatchAddMethod2();
	void showBatchAddMethod3();

	void centerElement(float width);

	Simulation* simulation;
	bool explorerMode = true;
	bool spawnedSprite = false;
	SDL_Texture* texture;
};