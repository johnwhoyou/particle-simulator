#pragma once

#include <imgui.h>
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "Simulation.h"
#include <random>
#include <iostream>

class MainGUI {
public:
	void Init(SDL_Window* window, const char* glsl_version);
	void NewFrame(SDL_Window* window);
	void Update(double frameRate);
	void Render();
	void Shutdown();

	void setSimulation(Simulation* sim) {
		simulation = sim;
	}

private:
	void displayCanvas();
	void displayBottomDetails(double frameRate);
	void displayParamsWindow();

	void showAddParticle();
	void showBatchAddParticle();
	void showBatchAddMethod1();
	void showBatchAddMethod2();
	void showBatchAddMethod3();
	void showAddWall();

	void centerElement(float width);

	Simulation* simulation;
};