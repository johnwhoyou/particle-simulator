#pragma once

#include <imgui.h>
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

class MainGUI {
public:
	void Init(SDL_Window* window, const char* glsl_version);
	void NewFrame(SDL_Window* window);
	void Update();
	void Render();
	void Shutdown();
	void showAddParticle();
	void showBatchAddParticle();
	void showBatchAddMethod1();
	void showBatchAddMethod2();
	void showBatchAddMethod3();
	void showAddWall();
	void centerElement(float width);
};