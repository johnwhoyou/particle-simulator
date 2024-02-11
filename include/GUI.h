#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class MainGUI {
public:
	void Init(GLFWwindow* window, const char* glsl_version);
	void NewFrame();
	virtual void Update();
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