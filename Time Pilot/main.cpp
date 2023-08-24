#include <iostream>

#include <Core/Window.h>

#include <Rendering/Light.h>
#include <Resource/ResourceManager.h>
#include <Rendering/Skybox.h>
#include <Rendering/Font.h>
#include <Rendering/FrameBuffer.h>

#include <Math/CommonMath.h>

#include <Game/Cube.h>
#include <Game/Projectile.h>
#include <Game/Globals.h>
#include <Game/FighterPlane.h>
#include <Game/FighterJet.h>
#include <Game/GameManager.h>
#include <Game/Hud.h>
#include <Game/Text2D.h>
#include <Game/Menu.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#define			IMGUI_GLSL_VERSION			"#version 130"

#define			DEFAULT_MOVE_SPEED			0.5f
#define			FAST_MOVE_SPEED				0.75f;

static Window* gWindow;


glm::vec3 CameraInitPos = glm::vec3(-34, 51, 101);
glm::vec3 CameraParentOffset = glm::vec3(0.0f);

float CameraParentZoom = 25.0f;
//bool CameraAttachedToParent = true;

Skybox* globalSkyBox;

Font* TextRasterizer;

Map<int, bool> KeyMap;

void InitFunction();
void RenderingFunction(GLFWwindow* window);
void InitializeImGui();
void RenderImGui();

void RenderObjectListTable(Object* object);
void ProcessInputs();

typedef struct DefaultTextData {
	float textX = 10.0f;
	float textY = 25.0f;
	float textScale = 0.5f;
	bool RenderText = true;
}
DefaultTextData;
DefaultTextData gTextData;


static int screenWidth, screenHeight;
static float rotationDelta = 45.0f;
static Light gLight;

void WindowKeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void WindowMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
void WindowMouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void WindowMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);



float AmbientStrength = 0.5f;
float DiffuseStrength = 0.5f;
float SpecularStrength = 1.0f;
float SpecularPower = 32.0f;

FighterJet* Player;
Hud* GameHud;
Menu* GameMenu;

bool HasGameFrozen = false;
bool ShouldRenderImGui = false;
int frames = 0;

float deltaTime = 0.0f;


Cube HydraCube(glm::vec3(0.0f), glm::vec3(75.0f, 75.0f, 400.0f));


static float mouseX = 0.0f;
static float mouseY = 0.0f;
static float moveSpeed = 30.0f;
static float rX, rY, rZ;
static std::string temp_window_title;


static FrameBuffer* FrameBuf;

int main(int argc, char** argv) {
	gWindow = new Window();	
	gWindow->Create("Time Pilot", 50, 50, 800, 600, false);
	gWindow->SetWindowFavIcon("./GameResources/Icons/window_favicon.png");
	gWindow->SetRenderingFunction(RenderingFunction);
	gWindow->ShowWindow();
	gWindow->SetWindowKeyboardCallback(WindowKeyPressCallback);
	gWindow->SetWindowMouseCursorCallback(WindowMouseMoveCallback);
	gWindow->SetWindowMouseButtonCallback(WindowMouseClickCallback);
	gWindow->SetWindowMouseScrollCallback(WindowMouseScrollCallback);
	InitFunction();
	gWindow->RenderLoop();
	return 0;
}

glm::vec3 original_scale;
void InitFunction() {
	Globals::gWindow = gWindow;
	gWindow->GetFrameBufferSize(&screenWidth, &screenHeight);
	Globals::gScreenWidth = screenWidth;
	Globals::gScreenHeight = screenHeight;

	FrameBuf = new FrameBuffer();
	FrameBuf->CreateFrameBuffer(Globals::gScreenWidth, Globals::gScreenHeight);
	Utils::InitRandomSeed();

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Globals::gFighterJetCube = &HydraCube;
	
	Globals::TextRasterizer = new Font();

	globalSkyBox = new Skybox("./GameResources/Skybox/Default", "png");
	Globals::InitializeGlobals();
	Globals::GetCameraInstance()->LookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Globals::GetCameraInstance()->SetProjectionType(ProjectionType::PERSPECTIVE);

	gWindow->GetFrameBufferSize(&screenWidth, &screenHeight);
	Globals::GetCameraInstance()->InitializePerspectiveParameters(70.0f, ((GLfloat)gWindow->GetScreenWidth() / (GLfloat)gWindow->GetScreenHeight()), Globals::gProjectionNear, Globals::gProjectionFar);
	glViewport(0, 0, gWindow->GetScreenWidth(), gWindow->GetScreenHeight());
	// =======================================================================================================
	// Framebuffer Shader
	// =======================================================================================================
	BaseShader* framebufferVS, *framebufferFS;
	framebufferVS = new BaseShader("Framebuffer Vertex Shader", "./GameResources/Shaders/FramebufferVertexShader.glsl", GL_VERTEX_SHADER);
	framebufferFS= new BaseShader("Framebuffer Fragment Shader", "./GameResources/Shaders/FramebufferFragmentShader.glsl", GL_FRAGMENT_SHADER);
	Shader* frameBufferShader = new Shader(std::vector<BaseShader*>({ framebufferVS, framebufferFS }));
	// =======================================================================================================
	// Sprite shader
	// =======================================================================================================
	BaseShader* spriteVertexShader, * spriteFragmentShader;
	spriteVertexShader = new BaseShader("Sprite Vertex Shader", "./GameResources/Shaders/SpriteVertexShader.glsl", GL_VERTEX_SHADER);
	spriteFragmentShader = new BaseShader("Sprite Fragment Shader", "./GameResources/Shaders/SpriteFragmentShader.glsl", GL_FRAGMENT_SHADER);
	Shader* SpriteShader = new Shader(std::vector<BaseShader*>({ spriteVertexShader, spriteFragmentShader }));
	Hud::gHudSpriteShader = SpriteShader;
	Menu::gMenuSpriteShader = SpriteShader;
	// =======================================================================================================
	// Basic Vertex, Fragment shader
	// =======================================================================================================
	BaseShader *vertexShader, *fragmentShader;
	vertexShader = new BaseShader("Base Vertex Shader", "./GameResources/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	fragmentShader = new BaseShader("Base Fragment Shader", "./GameResources/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	Shader* BasicShader = new Shader(std::vector<BaseShader*>({ vertexShader, fragmentShader }));
	FrameBuf->AttachShader(frameBufferShader);
	// =======================================================================================================
	// Skybox Shader
	// =======================================================================================================
	BaseShader* skyboxVS, *skyboxFS;
	skyboxVS = new BaseShader("Skybox Vertex Shader", "./GameResources/Shaders/SkyboxVS.glsl", GL_VERTEX_SHADER);
	skyboxFS = new BaseShader("Skybox Fragment Shader", "./GameResources/Shaders/SkyboxFS.glsl", GL_FRAGMENT_SHADER);
	Shader* SkyboxShader = new Shader(std::vector<BaseShader*>({ skyboxVS, skyboxFS }));
	globalSkyBox->SetShader(SkyboxShader);
	// =======================================================================================================
	// Text Shader
	// =======================================================================================================
	BaseShader* textVS, * textFS;
	textVS = new BaseShader("Text Vertex Shader", "./GameResources/Shaders/TextVertexShader.glsl", GL_VERTEX_SHADER);
	textFS = new BaseShader("Text Fragment Shader", "./GameResources/Shaders/TextFragmentShader.glsl", GL_FRAGMENT_SHADER);
	Shader* TextShader = new Shader(std::vector<BaseShader*>({ textVS, textFS }));
	
	Globals::TextRasterizer->LoadFont("./GameResources/Fonts/CascadiaMono.ttf");
	Globals::TextRasterizer->AssignShader(TextShader);

	Globals::FontRenderer = Globals::TextRasterizer;
	
	FighterPlane::LoadFighterPlaneDefaults("./GameResources/Models/EnemyPlanes/FighterPlane");
	Projectile::LoadMissileDefaults("./GameResources/Models/Missile");
	ProjectileDummy::LoadProjectileDummyDefaults("./GameResources/Models/Missile");
	FighterJet::LoadFighterJetDefaults("./GameResources/Models/FighterJet");

	FighterPlane::FighterPlaneMaterial->AttachShader(BasicShader);
	Projectile::ProjectileMaterial->AttachShader(BasicShader);
	ProjectileDummy::ProjectileDummyMaterial->AttachShader(BasicShader);
	FighterJet::HydraMaterial->AttachShader(BasicShader);
	GameMenu = new Menu();
	Menu::MenuFont->AssignShader(TextShader);
	Player = new FighterJet();
	Player->Load("./GameResources/Models/FighterJet");
	Player->SetVelocity(Globals::gFighterJetVelocity);
	
	GameHud = new Hud();

	Globals::CameraParentOffset = CameraInitPos - Player->FighterJetObject->GetTranslation();
	Globals::CameraParentOffset = glm::normalize(Globals::CameraParentOffset);
	Globals::GetCameraInstance()->LookAt(CameraInitPos, Player->FighterJetObject->GetTranslation(), glm::vec3(0.0f, 1.0f, 0.0f));

	gLight.SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
	gLight.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	gLight.SetDirection(glm::vec3(1.0f));

	
	double time = glfwGetTime();
	const int totalPlanes = 150;
	for (int i = 0; i < totalPlanes; i++) {
		FighterPlane* rustler = new FighterPlane();
		rustler->Load("./GameResources/Models/EnemyPlanes/FighterPlane");
		rustler->SetVelocity(Globals::gFighterPlaneVelocity);
	}
	double ptime = glfwGetTime();
	std::cout << "Total time taken to load " << totalPlanes << " planes: " << (ptime - time) << std::endl;

	InitializeImGui();
}

float inc_val = 0.0f;

void RenderingFunction(GLFWwindow* window) {
	// =======================================================================================================
	// Calculation of Deltatime
	// =======================================================================================================
	deltaTime = gWindow->GetDeltaTime();
	Globals::gDeltaTime = deltaTime;
	Globals::gGlfwTime = (float)glfwGetTime();

	Globals::gFramesPerSecond = gWindow->GetFramesPerSecond();

	temp_window_title = "TimePilot v0.1 Delta time: " + std::to_string(deltaTime) + " Frames: " + std::to_string(Globals::gFramesPerSecond);
	glfwSetWindowTitle(window, temp_window_title.c_str());
	// =======================================================================================================
	// Recalculate Projection Matrix
	// =======================================================================================================
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if (!Globals::IsUserInMenu && !Globals::IsGameOver) {
		FrameBuf->UpdateFrameBuffer(Globals::gScreenWidth, Globals::gScreenHeight);
		FrameBuf->BindBuffer();
		glEnable(GL_SCISSOR_TEST);
		gWindow->GetFrameBufferSize(&screenWidth, &screenHeight);
		
		Globals::gScreenWidth = screenWidth;
		Globals::gScreenHeight = screenHeight;
		if (Globals::gScreenWidth > 0 && Globals::gScreenHeight > 0) {
			Globals::GetCameraInstance()->SetProjectionType(ProjectionType::PERSPECTIVE);
			Globals::GetCameraInstance()->InitializePerspectiveParameters(70.0f, (static_cast<GLfloat>(Globals::gScreenWidth) / static_cast<GLfloat>(Globals::gScreenHeight)), Globals::gProjectionNear, Globals::gProjectionFar);
			
		}
		//Globals::GetCameraInstance()->SetProjectionType(ProjectionType::ORTHOGRAPHIC);
		//Globals::GetCameraInstance()->InitializePerspectiveParameters(70.0f, (static_cast<GLfloat>(Globals::gScreenWidth) / static_cast<GLfloat>(Globals::gScreenHeight)), Globals::gProjectionNear, Globals::gProjectionFar);
		//Globals::GetCameraInstance()->InitializeOrthographicParameters(static_cast<GLfloat>(Globals::gScreenWidth)/2.0f, -static_cast<GLfloat>(Globals::gScreenWidth)/2.0f, static_cast<GLfloat>(Globals::gScreenHeight)/2.0f, -static_cast<GLfloat>(Globals::gScreenHeight)/2.0f, -1000.0f, 1000.0f);
		
		glViewport(0, 0, screenWidth, screenHeight);
		glScissor(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		globalSkyBox->UseSkybox(Globals::GetCameraInstance());

		
		// =======================================================================================================
		// Render all the objects present in the resource manager..
		// =======================================================================================================
		for (Object* object : ResourceManager::gObjectList) {
			object->Render(Globals::GetCameraInstance());
			object->GetMaterial()->GetShader()->SetUniform3f("fLightColor", gLight.GetColor().x, gLight.GetColor().y, gLight.GetColor().z);
			object->GetMaterial()->GetShader()->SetUniform3f("fLightPosition", gLight.GetPosition().x, gLight.GetPosition().y, gLight.GetPosition().z);
			object->GetMaterial()->GetShader()->SetUniform3f("fCameraPosition", Globals::GetCameraInstance()->GetTranslation().x, Globals::GetCameraInstance()->GetTranslation().y, Globals::GetCameraInstance()->GetTranslation().z);
			object->GetMaterial()->GetShader()->SetUniform1f("fAmbientStrength", gLight.LightData.AmbientStrength);
			object->GetMaterial()->GetShader()->SetUniform1f("fDiffuseStrength", gLight.LightData.DiffuseStrength);
			object->GetMaterial()->GetShader()->SetUniform1f("fSpecularStrength", gLight.LightData.SpecularStrength);
			object->GetMaterial()->GetShader()->SetUniform1f("fSpecularPower", gLight.LightData.SpecularPower);
			
		}

		
		glDisable(GL_SCISSOR_TEST);
		
		if (Globals::gIsCameraAttachedToFighterJet && Globals::gShouldShowMiniMap) {
			glEnable(GL_SCISSOR_TEST);
			glViewport(screenWidth - 325, screenHeight - 215, 310, 200);
			glScissor(screenWidth - 325, screenHeight - 215, 310, 200);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			//// =======================================================================================================
			//// Render all the objects present in the resource manager..
			//// =======================================================================================================	
			Globals::GetCameraInstance()->SetProjectionType(ProjectionType::ORTHOGRAPHIC);
			Globals::GetCameraInstance()->InitializeOrthographicParameters(-30.0f, 30.0f, -19.0f, 19.0f, -100.0f, 100.0f);
			Globals::GetCameraInstance()->LookAt(Player->FighterJetObject->GetTranslation() + glm::vec3(0.0f, 40.0f, 1.0f), Player->FighterJetObject->GetTranslation(), glm::vec3(0.0f, 1.0f, 0.0f));

			for (Object* object : ResourceManager::gObjectList) {
				object->Render(Globals::GetCameraInstance());
				object->GetMaterial()->GetShader()->SetUniform3f("fLightColor", gLight.GetColor().x, gLight.GetColor().y, gLight.GetColor().z);
				object->GetMaterial()->GetShader()->SetUniform3f("fLightPosition", gLight.GetPosition().x, gLight.GetPosition().y, gLight.GetPosition().z);
				object->GetMaterial()->GetShader()->SetUniform3f("fCameraPosition", Globals::GetCameraInstance()->GetTranslation().x, Globals::GetCameraInstance()->GetTranslation().y, Globals::GetCameraInstance()->GetTranslation().z);
				object->GetMaterial()->GetShader()->SetUniform1f("fAmbientStrength", gLight.LightData.AmbientStrength);
				object->GetMaterial()->GetShader()->SetUniform1f("fDiffuseStrength", gLight.LightData.DiffuseStrength);
				object->GetMaterial()->GetShader()->SetUniform1f("fSpecularStrength", gLight.LightData.SpecularStrength);
				object->GetMaterial()->GetShader()->SetUniform1f("fSpecularPower", gLight.LightData.SpecularPower);
			}
			glDisable(GL_SCISSOR_TEST);
		}
		FrameBuf->UnbindBuffer();

		glViewport(0, 0, screenWidth, screenHeight);
		glScissor(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		FrameBuf->RenderFrameBuffer();
		FrameBuf->GetShader()->SetUniform1i("blur", Globals::gIsFighterJetBoosting ? 1 : 0);
		FrameBuf->GetShader()->SetUniform1i("highlight", Globals::IsHighlightOn ? 1 : (Globals::WasHighlightOn ? 1 : 0));
		
		if (Globals::IsHighlightOn) {
			Globals::HighlightAngle += 20.0f * Globals::gDeltaTime;
			if (Globals::HighlightAngle >= 90.0f) {
				Globals::HighlightAngle = 90.0f;
			}
		}
		else {
			if (Globals::WasHighlightOn) {
				Globals::HighlightAngle -= 20.0f * Globals::gDeltaTime;
				if (Globals::HighlightAngle <= 5.0f) {
					Globals::HighlightAngle = 5.0f;
					Globals::WasHighlightOn = false;
				}
			}
		}
		Globals::HighlightValue = glm::sin(glm::radians(Globals::HighlightAngle));
		FrameBuf->GetShader()->SetUniform1f("highlight_mul", Globals::HighlightValue);
		// =====================================================================================================
		// Call all behaviour functions - Basically how the objects behave
		// =====================================================================================================
		GameManager::CallBehaviouralFunctions();
		Projectile::CleanupMissileObjectsOnScene();
		//Behaviour::CleanUpBehaviourList();
		// =======================================================================================================
		// Draw UI text
		// =======================================================================================================
		if (gTextData.RenderText) {
			std::string text_string = "Camera Position (X: " + std::to_string(Globals::GetCameraInstance()->GetTranslation().x) + ", " + std::to_string(Globals::GetCameraInstance()->GetTranslation().y) + ", " + std::to_string(Globals::GetCameraInstance()->GetTranslation().z) + ")";
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, gTextData.textY, gTextData.textScale / 1.25, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "FPS: " + std::to_string(Globals::gFramesPerSecond);
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, gTextData.textY + 25, gTextData.textScale / 1.25, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "Camera Attached: " + std::string((Globals::gIsCameraAttachedToFighterJet == true) ? "Attached" : "Detached");
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, gTextData.textY + 50, gTextData.textScale / 1.25, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "Camera Freemode: " + std::string(Globals::gAllowMouseMovement ? "Enabled" : "Disabled");
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, gTextData.textY + 75, gTextData.textScale / 1.25, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "Controls:";
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, screenHeight - 25, gTextData.textScale / 1.75, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "Use arrow keys to rotate the plane";
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, screenHeight - 50, gTextData.textScale / 1.75, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "Press U key to detach the camera from the plane, press it again to re-attach.";
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, screenHeight - 75, gTextData.textScale / 1.75, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "(Note: this will put you in free camera mode, you can then use arrow keys to move around)";
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX + 25, screenHeight - 100, gTextData.textScale / 1.75, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "Press M key to toggle the plane translation.";
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, screenHeight - 125, gTextData.textScale / 1.75, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "Press C in free camera mode to toggle to cursor mode to use the ImGui functionality.";
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, screenHeight - 150, gTextData.textScale / 1.75, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "Press B to bring up the IMGUI debugging window.";
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, screenHeight - 175, gTextData.textScale / 1.75, glm::vec3(1.0f, 1.0f, 1.0f));
			text_string = "Press Left CTRL to shoot bullets.";
			Globals::TextRasterizer->RenderText(static_cast<float>(screenWidth), static_cast<float>(screenHeight), text_string, gTextData.textX, screenHeight - 200, gTextData.textScale / 1.75, glm::vec3(1.0f, 1.0f, 1.0f));
		}
		if (!HasGameFrozen) {
			if (Globals::gIsFighterJetBoosting) {
				Player->SetVelocity(Globals::gFighterJetBoostVelocity);

			}
			else {
				Player->SetVelocity(Globals::gFighterJetOriginalVelocity);
			}
		}
		//============================================================================================================
		// UI Drawing
		//============================================================================================================

		Globals::GetCameraInstance()->SetProjectionType(ProjectionType::ORTHOGRAPHIC);
		Globals::GetCameraInstance()->InitializeOrthographicParameters(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
		GameHud->DrawHud();
	}
	else {
		glViewport(0, 0, Globals::gScreenWidth, Globals::gScreenHeight);
		Globals::GetCameraInstance()->SetProjectionType(ProjectionType::ORTHOGRAPHIC);
		Globals::GetCameraInstance()->InitializeOrthographicParameters(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
		
		GameMenu->Draw();
	}
	ProcessInputs();
	if(ShouldRenderImGui) RenderImGui();
}

void InitializeImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFontAtlas* atlas = io.Fonts;
	ImFontConfig* config = new ImFontConfig;
	config->GlyphRanges = atlas->GetGlyphRangesDefault();
	config->PixelSnapH = true;
	atlas->AddFontFromFileTTF("./GameResources/Fonts/CascadiaMono.ttf", 14, config);
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(gWindow->GetWindow(), true);
	ImGui_ImplOpenGL3_Init(IMGUI_GLSL_VERSION);
}
void RenderImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Game Debugging Window");
	if (ImGui::CollapsingHeader("Global Light")) {
		if (ImGui::TreeNode("Position")) {
			if (ImGui::BeginTable("PositionAttributes", 3))
			{
				ImGui::TableNextColumn();
				ImGui::InputFloat("X", &gLight.Position.x);
				ImGui::TableNextColumn();
				ImGui::InputFloat("Y", &gLight.Position.y);
				ImGui::TableNextColumn();
				ImGui::InputFloat("Z", &gLight.Position.z);
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Color")) {
			if (ImGui::BeginTable("ColorAttributes", 3))
			{
				ImGui::TableNextColumn();
				ImGui::InputFloat("R", &gLight.Color.r);
				ImGui::TableNextColumn();
				ImGui::InputFloat("G", &gLight.Color.g);
				ImGui::TableNextColumn();
				ImGui::InputFloat("B", &gLight.Color.b);
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Parameters")) {
			if (ImGui::BeginTable("LightAttributes", 3))
			{
				ImGui::TableNextColumn();
				ImGui::Text("Light Attributes");
				ImGui::InputFloat("Ambient", &gLight.LightData.AmbientStrength);
				ImGui::InputFloat("Diffuse", &gLight.LightData.DiffuseStrength);
				ImGui::InputFloat("Specular", &gLight.LightData.SpecularStrength);
				ImGui::InputFloat("Specular Power", &gLight.LightData.SpecularPower);
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
	}
	if (ImGui::CollapsingHeader("Global Text")) {
		if (ImGui::TreeNode("Location")) {
			if (ImGui::BeginTable("PositionAttributes", 3))
			{
				ImGui::TableNextColumn();
				ImGui::InputFloat("X", &gTextData.textX);
				ImGui::TableNextColumn();
				ImGui::InputFloat("Y", &gTextData.textY);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::InputFloat("Scale", &gTextData.textScale);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Checkbox("Should Render", &gTextData.RenderText);
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
	}
	if (ImGui::CollapsingHeader("Sprites")) {
		for (Sprite* sprite : Sprite::gSpriteList) {
			if (ImGui::TreeNode(sprite->GetName().c_str())) {
				if (ImGui::BeginTable("PositionAttributes", 3))
				{
					ImGui::TableNextColumn();
					ImGui::InputFloat("X", &sprite->Translation.x);
					ImGui::TableNextColumn();
					ImGui::InputFloat("Y", &sprite->Translation.y);
					//ImGui::InputFloat("Z", &sprite->Translation.z);
					ImGui::EndTable();
				}
				if (ImGui::BeginTable("ColorAttributes", 3))
				{
					ImGui::TableNextColumn();
					ImGui::InputFloat("R", &sprite->mColor.r);
					ImGui::TableNextColumn();
					ImGui::InputFloat("G", &sprite->mColor.g);
					ImGui::TableNextColumn();
					ImGui::InputFloat("B", &sprite->mColor.b);
					//ImGui::InputFloat("Z", &sprite->Translation.z);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
		}
	}
	if (ImGui::CollapsingHeader("Objects")) {
		for (Object* object : ResourceManager::gObjectList) {

			if (object->GetParent() == nullptr) {
				RenderObjectListTable(object);
			}
		}
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void RenderObjectListTable(Object* object) {
	if(ImGui::TreeNode(object->GetName().c_str())) {
		if (ImGui::TreeNode("Attributes")) {
			if (ImGui::BeginTable("ObjectAttributes", 3))
			{
				// ================================================================================================
				// ImGui - Position
				// ================================================================================================
				ImGui::TableNextRow();
				ImGui::Text("Position");
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::InputFloat("pX", &object->Translation.x);
				ImGui::TableNextColumn();
				ImGui::InputFloat("pY", &object->Translation.y);
				ImGui::TableNextColumn();
				ImGui::InputFloat("pZ", &object->Translation.z);
				// ================================================================================================
				// ImGui - Rotation
				// ================================================================================================
				ImGui::TableNextRow();
				ImGui::Text("Rotation");
				ImGui::TableNextColumn();
				ImGui::TableNextColumn();
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::InputFloat("rX", &object->Rotation.x);
				ImGui::TableNextColumn();
				ImGui::InputFloat("rY", &object->Rotation.y);
				ImGui::TableNextColumn();
				ImGui::InputFloat("rZ", &object->Rotation.z);
				// ================================================================================================
				// ImGui - Scale
				// ================================================================================================
				ImGui::TableNextRow();
				ImGui::Text("Scale");
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::InputFloat("sX", &object->Scale.x);
				ImGui::TableNextColumn();
				ImGui::InputFloat("sY", &object->Scale.y);
				ImGui::TableNextColumn();
				ImGui::InputFloat("sZ", &object->Scale.z);
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
		// ================================================================================================
		// ImGui - Show transform matrix (Read only)
		// ================================================================================================
		if (ImGui::TreeNode("Object Transform (Readonly)")) {
			if (ImGui::BeginTable("TransformAttributes", 4))
			{
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						ImGui::TableNextColumn();
						ImGui::InputFloat(("m" + std::to_string(i) + std::to_string(j)).c_str(), &object->mTransform[i][j]);
					}

					ImGui::TableNextRow();
				}
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
		// ================================================================================================
		// ImGui - Child Objects
		// ================================================================================================
		if (object->GetChildObjects().size() > 0) {
			if (ImGui::TreeNode("Child Objects")) {
				for (Object* ChildObject : object->GetChildObjects()) {
					RenderObjectListTable(ChildObject);
				}
				ImGui::TreePop();
			}
		}

		// ================================================================================================
		// ImGui - Child Transforms
		// ================================================================================================
		if (object->GetChildTransforms().size() > 0) {
			if (ImGui::TreeNode("Child Transforms")) {
				for (Transform* transform : object->GetChildTransforms()) {
					if (ImGui::TreeNode(transform->GetName().c_str())) {
						if (ImGui::TreeNode("Transform Attributes")) {
							if (ImGui::BeginTable("PositionAttributes", 3))
							{
								// ================================================================================================
								// ImGui - Transform - Position
								// ================================================================================================
								ImGui::TableNextColumn();
								ImGui::Text("Position");
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::InputFloat("pX", &transform->Translation.x);
								ImGui::TableNextColumn();
								ImGui::InputFloat("pY", &transform->Translation.y);
								ImGui::TableNextColumn();
								ImGui::InputFloat("pZ", &transform->Translation.z);
								// ================================================================================================
								// ImGui - Transform - Rotation
								// ================================================================================================
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Rotation");
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::InputFloat("rX", &transform->Rotation.x);
								ImGui::TableNextColumn();
								ImGui::InputFloat("rY", &transform->Rotation.y);
								ImGui::TableNextColumn();
								ImGui::InputFloat("rZ", &transform->Rotation.z);
								// ================================================================================================
								// ImGui - Transform - Scale
								// ================================================================================================
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Scale");
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::InputFloat("sX", &transform->Scale.x);
								ImGui::TableNextColumn();
								ImGui::InputFloat("sY", &transform->Scale.y);
								ImGui::TableNextColumn();
								ImGui::InputFloat("sZ", &transform->Scale.z);
								ImGui::EndTable();
							}
							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
glm::vec3 MoveVector = glm::vec3(0.0f);

void WindowKeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	
	if (action == GLFW_REPEAT) {
		KeyMap.Set(key, true);
	}
	else if (action == GLFW_RELEASE) {
		KeyMap.Set(key, false);
	}
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE: {
				if (Globals::HasGameStarted) {
					Globals::IsUserInMenu = !Globals::IsUserInMenu;
					if (!Globals::IsUserInMenu)
						gWindow->HideCursor();
					else
						gWindow->ShowCursor();
				}
				break;
			}
			case GLFW_KEY_C: {
				if (gWindow->IsCursorHidden()) {
					gWindow->ShowCursor();
					Globals::gAllowMouseMovement = false;
				}
				else {
					gWindow->HideCursor();
					Globals::gAllowMouseMovement = true;
				}
				break;
			}
			case GLFW_KEY_L: {
				if (gWindow->IsMouseLockedAtOrigin()) gWindow->UnlockMouseFromOrigin();
				else gWindow->LockMouseAtOrigin();
				break;
			}
			case GLFW_KEY_U: {
				Globals::gIsCameraAttachedToFighterJet = !Globals::gIsCameraAttachedToFighterJet;
				break;
			}
			case GLFW_KEY_T: {
				ShouldRenderImGui = !ShouldRenderImGui;
				break;
			}
			case GLFW_KEY_Q: {
				Globals::gFighterPlaneAllRender = !Globals::gFighterPlaneAllRender;
				break;
			}
			case GLFW_KEY_M: {
				HasGameFrozen = !HasGameFrozen;
				if (Player->GetVelocity() > 1.0f) {
					Player->SetVelocity(0.0f);
					for (FighterPlane* rustler : FighterPlane::FighterPlaneList) {
						rustler->SetVelocity(0.0f);
					}
				}
				else {
					Player->SetVelocity(Globals::gFighterJetVelocity);
					for (FighterPlane* rustler : FighterPlane::FighterPlaneList) {
						rustler->SetVelocity(Globals::gFighterPlaneVelocity);
					}
				}
				break;
			}
			case GLFW_KEY_B: {
				if (!Globals::IsUserInMenu && Globals::HasGameStarted) {
					if (!Globals::gWasFighterJetBoosting) {
						Globals::gIsFighterJetBoosting = true;
					}
				}
				break;
			}
			case GLFW_KEY_V: {
				Globals::gShouldShowMiniMap = !Globals::gShouldShowMiniMap;
				break;
			}
			case GLFW_KEY_H: {
				if (!Globals::IsUserInMenu && Globals::HasGameStarted) {
					if (!Globals::gIsFighterJetBoosting) {
						Globals::IsHighlightOn = !Globals::IsHighlightOn;
						if (Globals::IsHighlightOn) Globals::HighlightAngle = 5.0f;
						if (!Globals::IsHighlightOn) {

							Globals::WasHighlightOn = true;
						}
					}
				}
				break;
			}
			case GLFW_KEY_LEFT_CONTROL: {
				if (!Globals::RefillingMissiles && !Globals::WasMissileFired) {
					Player->ShootMissile();
					Globals::CurrentMissiles--;
					Globals::WasMissileFired = true;
					if (Globals::CurrentMissiles < 0) {
						Globals::CurrentMissiles = 0;
					}
				}
			}

		}
	}
}
float y_rot = 0.0f;
void WindowMouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	if (!Globals::gIsCameraAttachedToFighterJet) {
		MouseData m_data;
		gWindow->GetMouseDelta(m_data, xpos, ypos);
		if (Globals::gAllowMouseMovement) {
			mouseX = m_data.mouseY * 0.05f;
			mouseY = -m_data.mouseX * 0.05f;
			Globals::GetCameraInstance()->RotateY(glm::radians(-m_data.mouseX * 0.05f));
			Globals::GetCameraInstance()->RotateX(glm::radians(m_data.mouseY * 0.05f));
		}
	}
	else {
		if (Globals::gAllowMouseMovement) {
			MouseData m_data;
			gWindow->GetMouseDelta(m_data, xpos, ypos);
			mouseX = m_data.mouseY * 0.05f;
			mouseY = -m_data.mouseX * 0.05f;
			glm::quat q0(glm::vec3(0.0f, glm::radians(mouseY), 0.0f));
			glm::quat q1 = glm::conjugate(q0);
			Globals::CameraParentOffset = q1 * Globals::CameraParentOffset * q0;
			q0 = glm::quat(glm::vec3(glm::radians(mouseX), 0.0f, 0.0f));
			q1 = glm::conjugate(q0);
			Globals::CameraParentOffset = q1 * Globals::CameraParentOffset * q0;
		}
	}
}

void ProcessInputs() {

	double xpos, ypos;
	gWindow->GetCursorPosition(&xpos, &ypos);
	Text2D::TextHoverChecks(xpos, ypos);
	

	if (Globals::gCameraBoost) Globals::gCameraMoveSpeed = Globals::gCameraOriginalMoveSpeed * Globals::gCameraMoveMultiplier;
	else Globals::gCameraMoveSpeed = Globals::gCameraOriginalMoveSpeed;
	MoveVector = glm::vec3(0.0f);
	if (KeyMap.Get(GLFW_KEY_UP) || KeyMap.Get(GLFW_KEY_W)) {
		if (!Globals::gIsCameraAttachedToFighterJet) {
			glm::vec3 NewPosition = (Globals::GetCameraInstance()->GetForward() * glm::vec3(Globals::gCameraMoveSpeed) * Globals::gDeltaTime);
			Globals::GetCameraInstance()->Translate(Globals::GetCameraInstance()->GetTranslation() + NewPosition);
		}
		else {
			Player->FighterJetObject->SetRotation(glm::vec3(Player->FighterJetObject->Rotation.x - (rotationDelta * Globals::gDeltaTime), Player->FighterJetObject->Rotation.y, Player->FighterJetObject->Rotation.z));
		}
	}
	if (KeyMap.Get(GLFW_KEY_DOWN) || KeyMap.Get(GLFW_KEY_S)) {
		if(!Globals::gIsCameraAttachedToFighterJet) {
			glm::vec3 NewPosition = -(Globals::GetCameraInstance()->GetForward() * glm::vec3(Globals::gCameraMoveSpeed) * Globals::gDeltaTime);
			Globals::GetCameraInstance()->Translate(Globals::GetCameraInstance()->GetTranslation() + NewPosition);
		}
		else {
			Player->FighterJetObject->SetRotation(glm::vec3(Player->FighterJetObject->Rotation.x + (rotationDelta * Globals::gDeltaTime), Player->FighterJetObject->Rotation.y, Player->FighterJetObject->Rotation.z));
		}
	}
	if (KeyMap.Get(GLFW_KEY_LEFT) || KeyMap.Get(GLFW_KEY_A)) {
		if (!Globals::gIsCameraAttachedToFighterJet) {
			glm::vec3 NewPosition = -(Globals::GetCameraInstance()->GetRight() * glm::vec3(Globals::gCameraMoveSpeed) * Globals::gDeltaTime);
			Globals::GetCameraInstance()->Translate(Globals::GetCameraInstance()->GetTranslation() + NewPosition);
			//MoveVector += NewPosition;
		} else {
			Player->FighterJetObject->SetRotation(glm::vec3(Player->FighterJetObject->Rotation.x, Player->FighterJetObject->Rotation.y + (rotationDelta * Globals::gDeltaTime) * ((int)(!KeyMap.Get(GLFW_KEY_SPACE))), Player->FighterJetObject->Rotation.z + (rotationDelta * Globals::gDeltaTime) * ((int)KeyMap.Get(GLFW_KEY_SPACE))));
		}
	}
	if (KeyMap.Get(GLFW_KEY_RIGHT) || KeyMap.Get(GLFW_KEY_D)) {
		if(!Globals::gIsCameraAttachedToFighterJet) {
			glm::vec3 NewPosition = +(Globals::GetCameraInstance()->GetRight() * glm::vec3(Globals::gCameraMoveSpeed) * Globals::gDeltaTime);
			Globals::GetCameraInstance()->Translate(Globals::GetCameraInstance()->GetTranslation() + NewPosition);
		} else {
			Player->FighterJetObject->SetRotation(glm::vec3(Player->FighterJetObject->Rotation.x, Player->FighterJetObject->Rotation.y - (rotationDelta * Globals::gDeltaTime) * ((int)(!KeyMap.Get(GLFW_KEY_SPACE))), Player->FighterJetObject->Rotation.z - (rotationDelta * Globals::gDeltaTime) * ((int)KeyMap.Get(GLFW_KEY_SPACE))));
		}
	}
	if (KeyMap.Get(GLFW_KEY_SPACE)) {
		if (!Globals::gIsCameraAttachedToFighterJet) {
			Globals::gCameraBoost = true;
		}
	}
	else {
		Globals::gCameraBoost = false;
	}
	if (!Globals::gIsCameraAttachedToFighterJet) {
		Globals::GetCameraInstance()->Translate(Globals::GetCameraInstance()->GetTranslation() + MoveVector);
	}
	if (KeyMap.Get(GLFW_MOUSE_BUTTON_RIGHT) || KeyMap.Get(GLFW_KEY_RIGHT_CONTROL)) {
		if (Globals::gGlfwTime - FighterJet::gPlayerFighterJet->LastBulletShotTime > (1.0f/ FighterJet::gPlayerFighterJet->BulletFiringRate)) {
			Player->ShootBullet();
			FighterJet::gPlayerFighterJet->LastBulletShotTime = Globals::gGlfwTime;
		}
	}
}

void WindowMouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (!gWindow->IsCursorHidden()) {
			GameManager::PollTextClickEvents(gWindow);
		}
		else
		{
			if (!Globals::RefillingMissiles && !Globals::WasMissileFired) {
				Player->ShootMissile();
				Globals::CurrentMissiles--;
				Globals::WasMissileFired = true;
				if (Globals::CurrentMissiles < 0) {
					Globals::CurrentMissiles = 0;
				}
			}
		}
	}
	switch (action) {
		case GLFW_PRESS: {
			KeyMap.Set(button, true);
			break;
		}
		case GLFW_RELEASE: {
			KeyMap.Set(button, false);
			break;
		}
	}
}
void WindowMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	spdlog::info("y_offset: " + std::to_string(yoffset));
	if (Globals::gIsCameraAttachedToFighterJet && !ShouldRenderImGui && gWindow->IsCursorHidden()) {
		Globals::CameraParentZoom -= 100.0f * yoffset * Globals::gDeltaTime;
	}
}