#include "GUI.h"

void MainGUI::Init(GLFWwindow* window, const char* glsl_version) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
}

void MainGUI::NewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void MainGUI::Update() {
	static bool batchAdd = false;
	float panelWidthPercentage = 0.25f;
	float panelWidth = ImGui::GetIO().DisplaySize.x * panelWidthPercentage;

	ImVec2 panelSize(panelWidth, ImGui::GetIO().DisplaySize.y);

	ImVec2 panelPosition(ImGui::GetIO().DisplaySize.x - panelWidth, 0);

	ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(panelPosition, ImGuiCond_Always);

	ImGui::Begin("STDISCM Particle Simulator", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowFontScale(1.5f);

	ImGui::Text("P1: Alessandra Pauleen Gomez");
	ImGui::Text("P2: John Carlo Joyo");

	ImGui::Dummy(ImVec2(0, 8));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 8));

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

	ImGui::Dummy(ImVec2(0, 8));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 8));

	ImGui::SetWindowFontScale(2.0f);
	centerElement(150.0f);
	ImGui::TextColored(ImVec4(0.7f, 0.7f, 1, 1), "FRAME RATE");
	centerElement(140.0f);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::SetWindowFontScale(1.5f);
	ImGui::End();
}

void MainGUI::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainGUI::Shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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
	ImGui::InputFloat("##x", &x, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##y", &y, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##angle", &angle, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##velocity", &velocity, 0.1f, 1.0f, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particle")) {
		// render particle to the simulation
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
	ImGui::SetColumnWidth(0, 150.0f);

	ImGui::Text("Value of n");
	ImGui::NextColumn();
	ImGui::InputInt("##numParticles", &numParticles, 1, 100);
	ImGui::NextColumn();

	ImGui::Text("Start X");
	ImGui::NextColumn();
	ImGui::InputFloat("##startX", &startX, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Start Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##startY", &startY, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("End X");
	ImGui::NextColumn();
	ImGui::InputFloat("##endX", &endX, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("End Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##endY", &endY, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##angle", &angle, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##velocity", &velocity, 0.1f, 1.0f, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particles")) {
		// render particles to the simulation
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
	ImGui::SetColumnWidth(0, 150.0f);

	ImGui::Text("Value of n");
	ImGui::NextColumn();
	ImGui::InputInt("##numParticles", &numParticles, 1, 100);
	ImGui::NextColumn();

	ImGui::Text("X");
	ImGui::NextColumn();
	ImGui::InputFloat("##x", &x, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##y", &y, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Start Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##startAngle", &startAngle, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("End Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##endAngle", &endAngle, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##velocity", &velocity, 0.1f, 1.0f, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particles")) {
		// render particles to the simulation
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
	ImGui::SetColumnWidth(0, 170.0f);

	ImGui::Text("Value of n");
	ImGui::NextColumn();
	ImGui::InputInt("##numParticles", &numParticles, 1, 100);
	ImGui::NextColumn();

	ImGui::Text("X");
	ImGui::NextColumn();
	ImGui::InputFloat("##x", &x, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y");
	ImGui::NextColumn();
	ImGui::InputFloat("##y", &y, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Angle");
	ImGui::NextColumn();
	ImGui::InputFloat("##angle", &angle, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Start Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##startVelocity", &startVelocity, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("End Velocity");
	ImGui::NextColumn();
	ImGui::InputFloat("##endVelocity", &endVelocity, 0.1f, 1.0f, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particles")) {
		// render particles to the simulation
	}
}

void MainGUI::showAddWall() {
	static float x1 = 0.0f;
	static float y1 = 0.0f;
	static float x2 = 0.0f;
	static float y2 = 0.0f;

	ImGui::Columns(2, "Wall Parameters", false);
	ImGui::SetColumnWidth(0, 100.0f);

	ImGui::Text("X1");
	ImGui::NextColumn();
	ImGui::InputFloat("##x1", &x1, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y1");
	ImGui::NextColumn();
	ImGui::InputFloat("####y1", &y1, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("X2");
	ImGui::NextColumn();
	ImGui::InputFloat("##x2", &x2, 0.1f, 1.0f, "%.2f");
	ImGui::NextColumn();

	ImGui::Text("Y2");
	ImGui::NextColumn();
	ImGui::InputFloat("##y2", &y2, 0.1f, 1.0f, "%.2f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(100.0f);
	if (ImGui::Button("Add Wall")) {
		// render wall to the simulation
	}
}

void MainGUI::centerElement(float width) {
	float panelWidth = ImGui::GetContentRegionAvail().x;
	float centerPosX = (panelWidth - width) * 0.5f;
	ImGui::SetCursorPosX(centerPosX);
}