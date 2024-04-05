#include "GUI.h"

std::random_device rd;
std::mt19937 eng(rd());

std::uniform_int_distribution<> distrX(0, 1280);
std::uniform_int_distribution<> distrY(0, 720);
std::uniform_real_distribution<> distrAngle(0, 360);
std::uniform_real_distribution<> distrVelocity(100, 500);

void MainGUI::Init(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* spriteTexture) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
	ImGui::StyleColorsDark();
	texture = spriteTexture;
}

void MainGUI::NewFrame(SDL_Window* window) {
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
}

void MainGUI::Update(double frameRate) {
	displayCanvas();
	displayBottomDetails(frameRate);
	displayControlsWindow();
}

void MainGUI::Render() {
	ImGui::Render();
}

void MainGUI::GetDrawData() {
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void MainGUI::Shutdown() {
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

bool MainGUI::getExplorerMode() {
	return explorerMode;
}

void MainGUI::displayCanvas() {
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(1300, 750), ImGuiCond_Always);
	ImGui::Begin("Simulation Canvas", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
												ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | 
												ImGuiWindowFlags_NoBringToFrontOnFocus);

	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImVec2(1280.0f, 720.0f);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y), IM_COL32(50, 50, 50, 255));

	Sprite sprite = simulation->getSprite();
	auto particles = simulation->getParticles();
	float particleSize = 2.5f;

	if (!explorerMode) {
		draw_list->AddRect(canvas_p0, ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y), IM_COL32(255, 255, 255, 255));

		for (const auto& particle : particles) {
			float posX = static_cast<float>(particle.getX()) - particleSize;
			float posY = canvas_sz.y - (static_cast<float>(particle.getY()) + particleSize);

			float adjustedPosX = std::max(particleSize, std::min(posX, canvas_sz.x - particleSize));
			float adjustedPosY = std::max(particleSize, std::min(posY, canvas_sz.y - particleSize));

			ImVec2 pos = ImVec2(canvas_p0.x + adjustedPosX, canvas_p0.y + adjustedPosY);
			ImVec2 squareSize = ImVec2(particleSize, particleSize);
			ImVec2 bottomRight = ImVec2(pos.x + squareSize.x, pos.y + squareSize.y);
			draw_list->AddRectFilled(pos, bottomRight, IM_COL32(255, 255, 0, 255));
		}

		if (spawnedSprite) {
			if (texture) {
				ImVec2 spriteSize = ImVec2(30.0f, 30.0f);
				ImVec2 spritePos = ImVec2(canvas_p0.x + sprite.getX() - (spriteSize.x / 2), canvas_p0.y + (canvas_sz.y - sprite.getY()) - (spriteSize.y / 2));
				draw_list->AddImage((void*)texture, spritePos, ImVec2(spritePos.x + spriteSize.x, spritePos.y + spriteSize.y));
			}
		}
	}
	else {
		float scaledWidth = 1280 / 33;
		float scaledHeight = 720 / 19;
		auto clientParticles = simulation->getClientParticles();
		for (const auto& particle : clientParticles) {
			ImVec2 pos = ImVec2(canvas_p0.x + particle.first , canvas_p0.y + (canvas_sz.y - particle.second));
			ImVec2 squareSize = ImVec2(scaledWidth, scaledHeight);
			ImVec2 bottomRight = ImVec2(pos.x + squareSize.x, pos.y + squareSize.y);
			draw_list->AddRectFilled(pos, bottomRight, IM_COL32(255, 255, 0, 255));
		}

		for (const auto& wall : simulation->getWalls()) {
			float distanceXWall = abs(wall.getX1() - sprite.getX());
			float distanceYWall = abs(wall.getY1() - (canvas_sz.y - sprite.getY()));

			if (distanceXWall <= 16 || distanceYWall <= 9) {
				float adjustedX1 = 640 + (wall.getX1() - sprite.getX()) * scaledWidth;
				float adjustedY1 = 360 + (wall.getY1() - (canvas_sz.y - sprite.getY())) * scaledHeight;
				float adjustedX2 = 640 + (wall.getX2() - sprite.getX()) * scaledWidth;
				float adjustedY2 = 360 + (wall.getY2() - (canvas_sz.y - sprite.getY())) * scaledHeight;

				ImVec2 pos1 = ImVec2(canvas_p0.x + adjustedX1, canvas_p0.y + adjustedY1);
				ImVec2 pos2 = ImVec2(canvas_p0.x + adjustedX2, canvas_p0.y + adjustedY2);
				draw_list->AddLine(pos1, pos2, IM_COL32(255, 255, 255, 255));

				if (wall.getX1() == wall.getX2())
					if (wall.getX1() == 0)
						draw_list->AddRectFilled(canvas_p0, pos2, IM_COL32(0, 0, 0, 255));
					else
						draw_list->AddRectFilled(ImVec2(pos1.x + 1, pos1.y), ImVec2(canvas_p0.x + canvas_sz.x, pos2.y), IM_COL32(0, 0, 0, 255));

				if (wall.getY1() == wall.getY2())
					if (wall.getY1() == 0)
						draw_list->AddRectFilled(canvas_p0, ImVec2(canvas_p0.x + canvas_sz.x, pos2.y), IM_COL32(0, 0, 0, 255));
					else
						draw_list->AddRectFilled(ImVec2(canvas_p0.x, pos1.y + 1), ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y), IM_COL32(0, 0, 0, 255));
			}
		}

		// self sprite
		if (texture) {
			ImVec2 spriteSize = ImVec2(55.0f, 55.0f);
			ImVec2 spritePos = ImVec2(canvas_p0.x + 640 - (spriteSize.x / 2), canvas_p0.y + 360 - (spriteSize.y / 2));
			draw_list->AddImage((void*)texture, spritePos, ImVec2(spritePos.x + spriteSize.x, spritePos.y + spriteSize.y));
		}

		// other sprites
		if (texture) {
			ImVec2 spriteSize = ImVec2(55.0f, 55.0f);
			auto clientSprites = simulation->getClientSprites();
			for (const auto& sprite : clientSprites) {
				ImVec2 spritePos = ImVec2(canvas_p0.x + sprite.getX() - (spriteSize.x / 2), canvas_p0.y + sprite.getY() - (spriteSize.y / 2));
				draw_list->AddImage((void*)texture, spritePos, ImVec2(spritePos.x + spriteSize.x, spritePos.y + spriteSize.y));
			}
		}
	}

	ImGui::End();
}

void MainGUI::displayBottomDetails(double frameRate) {
	ImGui::SetNextWindowPos(ImVec2(0, 720+50), ImGuiCond_Always);

	ImGui::Begin("Frame Rate", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
										ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	ImGui::SetWindowFontScale(2.0f);
	ImGui::TextColored(ImVec4(0.7f, 0.7f, 1, 1), "FRAME RATE:");
	ImGui::SameLine();
	ImGui::Text("%.1f FPS", frameRate);
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(16, 0));
	ImGui::SameLine();
	ImGui::Text("Particle Count: %zu", simulation->getParticles().size());
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(16, 0));


	ImGui::SameLine();
	ImGui::Text("Sprite Position: (%.2f, %.2f)", simulation->getSprite().getX(), simulation->getSprite().getY());
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(16, 0));
	

	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		simulation->clearParticles();
		simulation->resetSprite();
	}

	ImGui::SetWindowFontScale(1.5f);

	ImGui::Dummy(ImVec2(0, 4));
	ImGui::Text("P1: Alessandra Pauleen Gomez");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(16, 0));
	ImGui::SameLine();
	ImGui::Text("P2: John Carlo Joyo");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(16, 0));
	ImGui::SameLine();
	ImGui::Text("P3: Ibrahim Kahil");
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(16, 0));
	ImGui::SameLine();
	ImGui::Text("P4: Shaun Vincent Ong");

	ImGui::End();
}

void MainGUI::displayControlsWindow() {
	static bool batchAdd = false;

	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x - 1280, 0), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(1280+30, 0), ImGuiCond_Always);

	ImGui::Begin("Simulation Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

	ImGui::SetWindowFontScale(1.3f);

	//ImGui::Checkbox("Enable Explorer Mode", &explorerMode);

	ImGui::Dummy(ImVec2(0, 8));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 8));

	if (explorerMode) {
		ImGui::SetWindowFontScale(3.0f);

		centerElement(300.0f);
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 1, 1), "ARROW CONTROLS");
		ImGui::Dummy(ImVec2(0, 8));

		centerElement(74.0f);
		if (ImGui::ArrowButton("Up", ImGuiDir_Up)) {
			simulation->moveSprite('U');
		}

		centerElement(180.0f);
		if (ImGui::ArrowButton("Left", ImGuiDir_Left)) {
			simulation->moveSprite('L');
		}

		ImGui::SameLine();
		if (ImGui::ArrowButton("Down", ImGuiDir_Down)) {
			simulation->moveSprite('D');
		}

		ImGui::SameLine();
		if (ImGui::ArrowButton("Right", ImGuiDir_Right)) {
			simulation->moveSprite('R');
		}

		ImGui::SetWindowFontScale(1.3f);
		ImGui::Dummy(ImVec2(0, 8));
		centerElement(300.0f);
		ImGui::Text("ALTERNATIVE: WASD or Arrow Keys");
	}
	else {
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
		centerElement(150.0f);
		if (ImGui::Button("Clear Particles") && simulation) {
			simulation->clearParticles();
		}
	}

	ImGui::SetWindowFontScale(1.0f);
	ImGui::End();
}

void MainGUI::showAddParticle() {
	static double x = 0;
	static double y = 0;
	static double angle = 0.0;
	static double velocity = 0.0;

	ImGui::Columns(2, "Particle Parameters", false);
	ImGui::SetColumnWidth(0, 100.0f);

	ImGui::Text("X");
	ImGui::NextColumn();
	ImGui::InputDouble("##x", &x, 0, 0, "%.f");
	ImGui::NextColumn();

	ImGui::Text("Y");
	ImGui::NextColumn();
	ImGui::InputDouble("##y", &y, 0, 0, "%.f");
	ImGui::NextColumn();

	ImGui::Text("Angle");
	ImGui::NextColumn();
	ImGui::InputDouble("##angle", &angle, 0, 0, "%.1f");
	ImGui::NextColumn();

	ImGui::Text("Velocity");
	ImGui::NextColumn();
	ImGui::InputDouble("##velocity", &velocity, 0, 0, "%.1f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(250.0f);
	if (ImGui::Button("Add Particle") && simulation) {
		simulation->addParticle(x, y, angle, velocity);
	}

	ImGui::SameLine();
	if (ImGui::Button("Random Particle")) {
		x = distrX(eng);
		y = distrY(eng);
		angle = distrAngle(eng);
		velocity = distrVelocity(eng);
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
	static double startX = 0;
	static double startY = 0;
	static double endX = 0;
	static double endY = 0;
	static double angle = 0.0;
	static double velocity = 0.0;

	ImGui::Columns(2, "Particle Parameters", false);
	ImGui::SetColumnWidth(0, 120.0f);

	ImGui::Text("Value of n");
	ImGui::NextColumn();
	ImGui::InputInt("##numParticles", &numParticles, 0, 0);
	ImGui::NextColumn();

	ImGui::Text("Start X");
	ImGui::NextColumn();
	ImGui::InputDouble("##startX", &startX, 0, 0, "%.f");
	ImGui::NextColumn();

	ImGui::Text("Start Y");
	ImGui::NextColumn();
	ImGui::InputDouble("##startY", &startY, 0, 0, "%.f");
	ImGui::NextColumn();

	ImGui::Text("End X");
	ImGui::NextColumn();
	ImGui::InputDouble("##endX", &endX, 0, 0, "%.f");
	ImGui::NextColumn();

	ImGui::Text("End Y");
	ImGui::NextColumn();
	ImGui::InputDouble("##endY", &endY, 0, 0, "%.f");
	ImGui::NextColumn();

	ImGui::Text("Angle");
	ImGui::NextColumn();
	ImGui::InputDouble("##angle", &angle, 0, 0, "%.1f");
	ImGui::NextColumn();

	ImGui::Text("Velocity");
	ImGui::NextColumn();
	ImGui::InputDouble("##velocity", &velocity, 0, 0, "%.1f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particles") && simulation) {
		simulation->addParticleByBatchMethod1(startX, startY, endX, endY, angle, velocity, numParticles);
	}
}

void MainGUI::showBatchAddMethod2() {
	static int numParticles = 0;
	static double x = 0;
	static double y = 0;
	static double startAngle = 0.0;
	static double endAngle = 0.0;
	static double velocity = 0.0;

	ImGui::Columns(2, "Particle Parameters", false);
	ImGui::SetColumnWidth(0, 120.0f);

	ImGui::Text("Value of n");
	ImGui::NextColumn();
	ImGui::InputInt("##numParticles", &numParticles, 0, 0);
	ImGui::NextColumn();

	ImGui::Text("X");
	ImGui::NextColumn();
	ImGui::InputDouble("##x", &x, 0, 0, "%.f");
	ImGui::NextColumn();

	ImGui::Text("Y");
	ImGui::NextColumn();
	ImGui::InputDouble("##y", &y, 0, 0, "%.f");
	ImGui::NextColumn();

	ImGui::Text("Start Angle");
	ImGui::NextColumn();
	ImGui::InputDouble("##startAngle", &startAngle, 0, 0, "%.1f");
	ImGui::NextColumn();

	ImGui::Text("End Angle");
	ImGui::NextColumn();
	ImGui::InputDouble("##endAngle", &endAngle, 0, 0, "%.1f");
	ImGui::NextColumn();

	ImGui::Text("Velocity");
	ImGui::NextColumn();
	ImGui::InputDouble("##velocity", &velocity, 0, 0, "%.1f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particles") && simulation) {
		simulation->addParticleByBatchMethod2(x, y, startAngle, endAngle, velocity, numParticles);
	}
}

void MainGUI::showBatchAddMethod3() {
	static int numParticles = 0;
	static int x = 0.0;
	static int y = 0.0;
	static double angle = 0.0;
	static double startVelocity = 0.0;
	static double endVelocity = 0.0;

	ImGui::Columns(2, "Particle Parameters", false);
	ImGui::SetColumnWidth(0, 150.0f);

	ImGui::Text("Value of n");
	ImGui::NextColumn();
	ImGui::InputInt("##numParticles", &numParticles, 0, 0);
	ImGui::NextColumn();

	ImGui::Text("X");
	ImGui::NextColumn();
	ImGui::InputInt("##x", &x, 0, 0);
	ImGui::NextColumn();

	ImGui::Text("Y");
	ImGui::NextColumn();
	ImGui::InputInt("##y", &y, 0, 0);
	ImGui::NextColumn();

	ImGui::Text("Angle");
	ImGui::NextColumn();
	ImGui::InputDouble("##angle", &angle, 0, 0, "%.1f");
	ImGui::NextColumn();

	ImGui::Text("Start Velocity");
	ImGui::NextColumn();
	ImGui::InputDouble("##startVelocity", &startVelocity, 0, 0, "%.1f");
	ImGui::NextColumn();

	ImGui::Text("End Velocity");
	ImGui::NextColumn();
	ImGui::InputDouble("##endVelocity", &endVelocity, 0, 0, "%.1f");
	ImGui::Columns(1);
	ImGui::Dummy(ImVec2(0, 8));

	centerElement(150.0f);
	if (ImGui::Button("Add Particles") && simulation) {
		simulation->addParticleByBatchMethod3(x, y, angle, startVelocity, endVelocity, numParticles);
	}
}

void MainGUI::centerElement(float width) {
	float panelWidth = ImGui::GetContentRegionAvail().x;
	float centerPosX = (panelWidth - width) * 0.5f;
	ImGui::SetCursorPosX(centerPosX);
}