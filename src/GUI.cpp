#include "GUI.h"

void MainGUI::Init(SDL_Window* window, const char* glsl_version) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	ImGui_ImplSDL2_InitForOpenGL(window, nullptr);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
}

void MainGUI::NewFrame(SDL_Window* window) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
}

void MainGUI::Update() {
	displayCanvas();
	displayBottomDetails();
	displayParamsWindow();
}

void MainGUI::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainGUI::Shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void MainGUI::displayCanvas() {
    ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

    ImGui::Begin("Simulation Canvas", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    // Drawing a simple rectangle
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // Top-left corner of the canvas
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + 1280, canvas_p0.y + 720); // Bottom-right corner of the canvas

    // Coordinates for the rectangle
    ImVec2 rect_p0 = ImVec2(canvas_p0.x + 100, canvas_p0.y + 100);
    ImVec2 rect_p1 = ImVec2(canvas_p0.x + 200, canvas_p0.y + 200);

    // Draw rectangle
    draw_list->AddRectFilled(rect_p0, rect_p1, IM_COL32(255, 0, 0, 255)); // Red rectangle

    ImGui::End();
}

void MainGUI::displayBottomDetails() {
	ImGui::SetNextWindowPos(ImVec2(0, 720), ImGuiCond_Always);

	ImGui::Begin("Frame Rate", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
										ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	ImGui::SetWindowFontScale(2.0f);
	ImGui::TextColored(ImVec4(0.7f, 0.7f, 1, 1), "FRAME RATE:");
	ImGui::SameLine();
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(16, 0));
	ImGui::SetWindowFontScale(1.5f);

	ImGui::SameLine();
	if (ImGui::Button("Clear Particles")) {
		// TODO: clear all particles
	}

	ImGui::SameLine();
	if (ImGui::Button("Clear Walls")) {
		// TODO: clear all walls
	}

	ImGui::SameLine();
	if (ImGui::Button("Clear All")) {
		// TODO: clear all particles and walls
	}

	ImGui::Text("P1: Alessandra Pauleen Gomez");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(16, 0));
	ImGui::SameLine();
	ImGui::Text("P2: John Carlo Joyo");

	ImGui::End();
}

void MainGUI::displayParamsWindow() {
	static bool batchAdd = false;

	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - 1280, 0), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(1280, 0), ImGuiCond_Always);

	ImGui::Begin("Simulation Parameters", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | 
	ImGuiWindowFlags_NoBackground);

	ImGui::SetWindowFontScale(1.3f);

	centerElement(150.0f);
	ImGui::TextColored(ImVec4(0.7f, 0.7f, 1, 1), "ADD PARTICLE/S");
	ImGui::Spacing();
	ImGui::Checkbox("Add Particles by Batch", &batchAdd);
	ImGui::Dummy(ImVec2(0, 8));

	if (!batchAdd)
		showAddParticle();
	else
		showBatchAddParticle();

	ImGui::Dummy(ImVec2(0, 8));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(100.0f);
	ImGui::TextColored(ImVec4(0.7f, 0.7f, 1, 1), "ADD WALL");
	ImGui::Dummy(ImVec2(0, 8));
	showAddWall();

	ImGui::SetWindowFontScale(1.0f);
	ImGui::End();
}

void MainGUI::showAddParticle() {
	static float x = 0.0f;
	static float y = 0.0f;
	static float angle = 0.0f;
	static float velocity = 0.0f;
	
	ImGui::Columns(2, "Particle Parameters", false);
	ImGui::SetColumnWidth(0, 100.0f);

	ImGui::Text("X");
	ImGui::NextColumn();
	ImGui::InputFloat("##x", &x, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##y", &y, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##angle", &angle, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##velocity", &velocity, 0, 0, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particle")) {
		// TODO: add particle to the simulation
	}
}

void MainGUI::showBatchAddParticle() {
	static const char* items[] = {"Method 1", "Method 2", "Method 3"};
	static int currentItem = 0;

	ImGui::Combo(" ", &currentItem, items, IM_ARRAYSIZE(items));
	ImGui::Dummy(ImVec2(0, 8));

	switch (currentItem) {
		case 0:
			showBatchAddMethod1(); break;
		case 1:
			showBatchAddMethod2(); break;
		case 2:
			showBatchAddMethod3(); break;
		default: break;
	}
}

void MainGUI::showBatchAddMethod1() {
	static int numParticles = 0;
	static float startX = 0.0f;
	static float startY = 0.0f;
	static float endX = 0.0f;
	static float endY = 0.0f;
	static float angle = 0.0f;
	static float velocity = 0.0f;

	ImGui::Columns(2, "Particle Parameters", false);
	ImGui::SetColumnWidth(0, 120.0f);

	ImGui::Text("Value of n");
	ImGui::NextColumn();
	ImGui::InputInt("##numParticles", &numParticles, 0, 0);
	ImGui::NextColumn();

	ImGui::Text("Start X");
	ImGui::NextColumn();
	ImGui::InputFloat("##startX", &startX, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Start Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##startY", &startY, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("End X");
	ImGui::NextColumn();
	ImGui::InputFloat("##endX", &endX, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("End Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##endY", &endY, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##angle", &angle, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##velocity", &velocity, 0, 0, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particles")) {
		// TODO: add particles to the simulation
	}
}

void MainGUI::showBatchAddMethod2() {
	static int numParticles = 0;
	static float x = 0.0f;
	static float y = 0.0f;
	static float startAngle = 0.0f;
	static float endAngle = 0.0f;
	static float velocity = 0.0f;

	ImGui::Columns(2, "Particle Parameters", false);
	ImGui::SetColumnWidth(0, 120.0f);

	ImGui::Text("Value of n");
	ImGui::NextColumn();
	ImGui::InputInt("##numParticles", &numParticles, 0, 0);
	ImGui::NextColumn();

	ImGui::Text("X");
	ImGui::NextColumn();
	ImGui::InputFloat("##x", &x, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##y", &y, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Start Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##startAngle", &startAngle, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("End Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##endAngle", &endAngle, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##velocity", &velocity, 0, 0, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particles")) {
		// TODO: add particles to the simulation
	}
}

void MainGUI::showBatchAddMethod3() {
	static int numParticles = 0;
	static float x = 0.0f;
	static float y = 0.0f;
	static float angle = 0.0f;
	static float startVelocity = 0.0f;
	static float endVelocity = 0.0f;

	ImGui::Columns(2, "Particle Parameters", false);
	ImGui::SetColumnWidth(0, 150.0f);

	ImGui::Text("Value of n");
	ImGui::NextColumn();
	ImGui::InputInt("##numParticles", &numParticles, 0, 0);
	ImGui::NextColumn();

	ImGui::Text("X");
	ImGui::NextColumn();
	ImGui::InputFloat("##x", &x, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##y", &y, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##angle", &angle, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Start Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##startVelocity", &startVelocity, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("End Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##endVelocity", &endVelocity, 0, 0, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particles")) {
		// TODO: add particles to the simulation
	}
}

void MainGUI::showAddWall() {
	static float x1 = 0.0f;
	static float y1 = 0.0f;
	static float x2 = 0.0f;
	static float y2 = 0.0f;

	ImGui::Columns(2, "Wall Parameters", false);
	ImGui::SetColumnWidth(0, 50.0f);

	ImGui::Text("X1");
	ImGui::NextColumn();
	ImGui::InputFloat("##x1", &x1, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y1");
	ImGui::NextColumn();
	ImGui::InputFloat("####y1", &y1, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("X2");
	ImGui::NextColumn();
	ImGui::InputFloat("##x2", &x2, 0, 0, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y2");
	ImGui::NextColumn();
	ImGui::InputFloat("##y2", &y2, 0, 0, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(100.0f);
	if (ImGui::Button("Add Wall")) {
		// TODO: add wall to the simulation
	}
}

void MainGUI::centerElement(float width) {
	float panelWidth = ImGui::GetContentRegionAvail().x;
	float centerPosX = (panelWidth - width) * 0.5f;
	ImGui::SetCursorPosX(centerPosX);
}