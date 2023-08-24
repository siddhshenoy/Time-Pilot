//#include <spdlog/spdlog.h>
//
//#include <imgui/imgui.h>
//#include <imgui/backends/imgui_impl_glfw.h>
//#include <imgui/backends/imgui_impl_opengl3.h>
//
//#define			IMGUI_GLSL_VERSION			"#version 130"
//
//#include <Core/Window.h>
//#include <Rendering/Light.h>
//#include <Resource/ResourceManager.h>
//
//Texture* TeapotTexture;
//
//Light* LightObject;
//
//typedef struct TeapotObject {
//	bool ApplyTexture;
//	Object* Teapot;
//	Shader* TeapotShader;
//	Material* TeapotMat;
//} TeapotObject;
//
//TeapotObject TeapotObjects[4];
//
//typedef struct CookTorranceLightData {
//	float F0 = 0.8f;
//	float Roughness = 0.1f;
//	float k = 0.2f;
//}
//CookTorranceLightData;
//CookTorranceLightData CTLightData;
//
//typedef struct OrenNayarLightData {
//	float Roughness = 1.0f;
//	float Albedo = 1.0f;
//	float Irradiance = 1.0f;
//}
//OrenNayarLightData;
//OrenNayarLightData ONLightData;
//
//Camera* GlobalCamera;
//
//float fRoughness = 1.0f;
//float fAlbedo = 1.0f;
//float fIrradiance = 1.0f;
//float fIntensityMultiplier = 1.0f;
//
//bool ApplyTextures = true;
//
//void InitFunction();
//void RenderingFunction(GLFWwindow* window);
//void InitializeImGui();
//void RenderImGui();
//void RenderObjectListTable(Object* object);
//Window* gWindow;
//
//
//int main() {
//	gWindow = new Window();
//	gWindow->Create("Real Time Rendering - Lab 1", 50, 50, 800, 600, false);
//	//gWindow->SetWindowFavIcon("./GameResources/Icons/window_favicon.png");
//	gWindow->SetRenderingFunction(RenderingFunction);
//	gWindow->ShowWindow();
//	/*gWindow->SetWindowKeyboardCallback(WindowKeyPressCallback);
//	gWindow->SetWindowMouseCursorCallback(WindowMouseMoveCallback);
//	gWindow->SetWindowMouseButtonCallback(WindowMouseClickCallback);
//	gWindow->SetWindowMouseScrollCallback(WindowMouseScrollCallback);*/
//	InitFunction();
//	gWindow->RenderLoop();
//}
//
//void InitFunction() {
//
//	// Enable Depth Test
//	glEnable(GL_DEPTH_TEST);
//
//	// Enable back face culling
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//
//	GlobalCamera = new Camera();
//	
//	GlobalCamera->SetProjectionType(ProjectionType::PERSPECTIVE);
//	GlobalCamera->Translate(glm::vec3(0.0f, 0.0f, 10.0f));
//	GlobalCamera->LookAt(glm::vec3(0.0f, 2.5f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//	LightObject = new Light();
//	LightObject->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
//	LightObject->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
//
//	TeapotTexture = new Texture();
//	TeapotTexture->LoadTexture("./GameResources/Models/Extra/Teapot_Texture.jpg");
//
//	BaseShader* PhongVertexShader, * PhongFragmentShader;
//	PhongVertexShader = new BaseShader("Phong Vertex Shader", "./GameResources/Shaders/PhongVertexShader.glsl", GL_VERTEX_SHADER);
//	PhongFragmentShader = new BaseShader("Phong Fragment Shader", "./GameResources/Shaders/PhongFragmentShader.glsl", GL_FRAGMENT_SHADER);
//	TeapotObjects[0].TeapotShader = new Shader(std::vector<BaseShader*>({ PhongVertexShader,PhongFragmentShader }));
//	TeapotObjects[0].Teapot = ResourceManager::LoadObject("./GameResources/Models/Extra/Teapot.fbx");
//	TeapotObjects[0].Teapot->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
//	TeapotObjects[0].Teapot->SetTranslation(glm::vec3(3.0f, 0.0f, 0.0f));
//	TeapotObjects[0].TeapotMat = new Material();
//	TeapotObjects[0].TeapotMat->AttachShader(TeapotObjects[0].TeapotShader);
//	TeapotObjects[0].TeapotMat->AttachTexture(TeapotTexture);
//	TeapotObjects[0].Teapot->AssignMaterial(TeapotObjects[0].TeapotMat);
//	TeapotObjects[0].ApplyTexture = true;
//
//
//	BaseShader* ToonVertexShader, * ToonFragmentShader;
//	ToonVertexShader = new BaseShader("Toon Vertex Shader", "./GameResources/Shaders/ToonVertexShader.glsl", GL_VERTEX_SHADER);
//	ToonFragmentShader = new BaseShader("Toon Fragment Shader", "./GameResources/Shaders/ToonFragmentShader.glsl", GL_FRAGMENT_SHADER);
//	TeapotObjects[1].TeapotShader = new Shader(std::vector<BaseShader*>({ ToonVertexShader,ToonFragmentShader}));
//	TeapotObjects[1].Teapot = ResourceManager::LoadObject("./GameResources/Models/Extra/Teapot.fbx");
//	TeapotObjects[1].Teapot->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
//	TeapotObjects[1].Teapot->SetTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
//	TeapotObjects[1].TeapotMat = new Material();
//	TeapotObjects[1].TeapotMat->AttachShader(TeapotObjects[1].TeapotShader);
//	TeapotObjects[1].TeapotMat->AttachTexture(TeapotTexture);
//	TeapotObjects[1].Teapot->AssignMaterial(TeapotObjects[1].TeapotMat);
//	TeapotObjects[1].ApplyTexture = true;
//
//	BaseShader* ONVertexShader, * ONFragmentShader;
//	ONVertexShader = new BaseShader("Toon Vertex Shader", "./GameResources/Shaders/OrenNayarVertexShader.glsl", GL_VERTEX_SHADER);
//	ONFragmentShader = new BaseShader("Toon Fragment Shader", "./GameResources/Shaders/OrenNayarFragmentShader.glsl", GL_FRAGMENT_SHADER);
//	TeapotObjects[2].TeapotShader = new Shader(std::vector<BaseShader*>({ ONVertexShader,ONFragmentShader }));
//	TeapotObjects[2].Teapot = ResourceManager::LoadObject("./GameResources/Models/Extra/Teapot.fbx");
//	TeapotObjects[2].Teapot->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
//	TeapotObjects[2].Teapot->SetTranslation(glm::vec3(-3.0f, 0.0f, 0.0f));
//	TeapotObjects[2].TeapotMat = new Material();
//	TeapotObjects[2].TeapotMat->AttachShader(TeapotObjects[2].TeapotShader);
//	TeapotObjects[2].TeapotMat->AttachTexture(TeapotTexture);
//	TeapotObjects[2].Teapot->AssignMaterial(TeapotObjects[2].TeapotMat);
//	TeapotObjects[2].ApplyTexture = true;
//
//
//	BaseShader* CTVertexShader, * CTFragmentShader;
//	CTVertexShader = new BaseShader("Toon Vertex Shader", "./GameResources/Shaders/CookTorranceVertexShader.glsl", GL_VERTEX_SHADER);
//	CTFragmentShader = new BaseShader("Toon Fragment Shader", "./GameResources/Shaders/CookTorranceFragmentShader.glsl", GL_FRAGMENT_SHADER);
//	TeapotObjects[3].TeapotShader = new Shader(std::vector<BaseShader*>({ CTVertexShader,CTFragmentShader }));
//	TeapotObjects[3].Teapot = ResourceManager::LoadObject("./GameResources/Models/Extra/Teapot.fbx");
//	TeapotObjects[3].Teapot->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
//	TeapotObjects[3].Teapot->SetTranslation(glm::vec3(-3.0f, 0.0f, 3.0f));
//	TeapotObjects[3].TeapotMat = new Material();
//	TeapotObjects[3].TeapotMat->AttachShader(TeapotObjects[3].TeapotShader);
//	TeapotObjects[3].TeapotMat->AttachTexture(TeapotTexture);
//	TeapotObjects[3].Teapot->AssignMaterial(TeapotObjects[3].TeapotMat);
//	TeapotObjects[3].ApplyTexture = true;
//
//	InitializeImGui();
//
//}
//
//void RenderingFunction(GLFWwindow* window) {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//
//	int windowWidth, windowHeight;
//	gWindow->GetDimensions(&windowWidth, &windowHeight);
//	GlobalCamera->InitializePerspectiveParameters(70.0f, ((float)windowWidth / (float)windowHeight), 0.001f, 1000.0f);
//
//	for (int i = 0; i < 4; i++) {
//		TeapotObjects[i].ApplyTexture = ApplyTextures;
//	}
//	for (Object* object : ResourceManager::gObjectList) {
//		object->Render(GlobalCamera);
//		object->GetMaterial()->GetShader()->SetUniform3f("fLightColor", LightObject->GetColor().x, LightObject->GetColor().y, LightObject->GetColor().z);
//		object->GetMaterial()->GetShader()->SetUniform3f("fLightPosition", LightObject->GetPosition().x, LightObject->GetPosition().y, LightObject->GetPosition().z);
//		object->GetMaterial()->GetShader()->SetUniform3f("fCameraPosition", GlobalCamera->GetTranslation().x, GlobalCamera->GetTranslation().y, GlobalCamera->GetTranslation().z);
//		object->GetMaterial()->GetShader()->SetUniform1f("fAmbientStrength", LightObject->LightData.AmbientStrength);
//		object->GetMaterial()->GetShader()->SetUniform1f("fDiffuseStrength", LightObject->LightData.DiffuseStrength);
//		object->GetMaterial()->GetShader()->SetUniform1f("fSpecularStrength", LightObject->LightData.SpecularStrength);
//		object->GetMaterial()->GetShader()->SetUniform1f("fSpecularPower", LightObject->LightData.SpecularPower);
//		object->GetMaterial()->GetShader()->SetUniform1f("fRoughness", ONLightData.Roughness);
//		object->GetMaterial()->GetShader()->SetUniform1f("fAlbedo", ONLightData.Albedo);
//		object->GetMaterial()->GetShader()->SetUniform1f("fIrradiance", ONLightData.Irradiance);
//		object->GetMaterial()->GetShader()->SetUniform1i("fApplyTexture", ApplyTextures ? 1 : 0);
//		object->GetMaterial()->GetShader()->SetUniform1f("fIntensityMultiplier", fIntensityMultiplier);
//		object->GetMaterial()->GetShader()->SetUniform1f("F0", CTLightData.F0);
//		object->GetMaterial()->GetShader()->SetUniform1f("roughness", CTLightData.Roughness);
//		object->GetMaterial()->GetShader()->SetUniform1f("k", CTLightData.k);
//		object->SetRotation(object->GetRotation() + glm::vec3(0.0f, 30.0f * gWindow->GetDeltaTime(), 0.0f));
//	}
//	RenderImGui();
//}
//
//
//void InitializeImGui() {
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	ImFontAtlas* atlas = io.Fonts;
//	ImFontConfig* config = new ImFontConfig;
//	config->GlyphRanges = atlas->GetGlyphRangesDefault();
//	config->PixelSnapH = true;
//	atlas->AddFontFromFileTTF("./GameResources/Fonts/CascadiaMono.ttf", 14, config);
//	ImGui::StyleColorsDark();
//	ImGui_ImplGlfw_InitForOpenGL(gWindow->GetWindow(), true);
//	ImGui_ImplOpenGL3_Init(IMGUI_GLSL_VERSION);
//}
//void RenderImGui() {
//	ImGui_ImplOpenGL3_NewFrame();
//	ImGui_ImplGlfw_NewFrame();
//	ImGui::NewFrame();
//	ImGui::Begin("Game Debugging Window");
//	if (ImGui::CollapsingHeader("Global Light")) {
//		if (ImGui::TreeNode("Position")) {
//			if (ImGui::BeginTable("PositionAttributes", 3))
//			{
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("X", &LightObject->Position.x);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("Y", &LightObject->Position.y);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("Z", &LightObject->Position.z);
//				ImGui::EndTable();
//			}
//			ImGui::TreePop();
//		}
//		if (ImGui::TreeNode("Color")) {
//			if (ImGui::BeginTable("ColorAttributes", 3))
//			{
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("R", &LightObject->Color.r);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("G", &LightObject->Color.g);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("B", &LightObject->Color.b);
//				ImGui::EndTable();
//			}
//			ImGui::TreePop();
//		}
//		if (ImGui::TreeNode("Parameters")) {
//			if (ImGui::BeginTable("LightAttributes", 1))
//			{
//				ImGui::TableNextColumn();
//				ImGui::Text("Default Light Attributes");
//				ImGui::InputFloat("Ambient", &LightObject->LightData.AmbientStrength);
//				ImGui::InputFloat("Diffuse", &LightObject->LightData.DiffuseStrength);
//				ImGui::InputFloat("Specular", &LightObject->LightData.SpecularStrength);
//				ImGui::InputFloat("Specular Power", &LightObject->LightData.SpecularPower);
//				ImGui::Text("Oren-Nayar Lighting Parameters");
//				ImGui::InputFloat("Roughness", &ONLightData.Roughness);
//				ImGui::InputFloat("Irradiance", &ONLightData.Irradiance);
//				ImGui::InputFloat("Albedo", &ONLightData.Albedo);
//				ImGui::Text("Toon Lighting Parameters");
//				ImGui::InputFloat("Intensity Multiplier", &fIntensityMultiplier);
//				ImGui::Text("Cook-Torrance Lighting Parameters");
//				ImGui::InputFloat("F0", &CTLightData.F0);
//				ImGui::InputFloat("CT-Roughness", &CTLightData.Roughness);
//				ImGui::InputFloat("K", &CTLightData.k);
//				ImGui::EndTable();
//			}
//			ImGui::TreePop();
//		}
//		if (ImGui::TreeNode("Object Material Attributes")) {
//			if (ImGui::BeginTable("MaterialAttributes", 1))
//			{
//				ImGui::TableNextColumn();
//				ImGui::Checkbox("Apply Textures", &ApplyTextures);
//				
//				ImGui::EndTable();
//			}
//			ImGui::TreePop();
//		}
//	}
//
//
//	if (ImGui::CollapsingHeader("Objects")) {
//		for (Object* object : ResourceManager::gObjectList) {
//
//			if (object->GetParent() == nullptr) {
//				RenderObjectListTable(object);
//			}
//		}
//	}
//	ImGui::End();
//	ImGui::Render();
//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//}
//void RenderObjectListTable(Object* object) {
//	if(ImGui::TreeNode(object->GetName().c_str())) {
//		if (ImGui::TreeNode("Attributes")) {
//			if (ImGui::BeginTable("ObjectAttributes", 3))
//			{
//				// ================================================================================================
//				// ImGui - Position
//				// ================================================================================================
//				ImGui::TableNextRow();
//				ImGui::Text("Position");
//				ImGui::TableNextRow();
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("pX", &object->Translation.x);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("pY", &object->Translation.y);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("pZ", &object->Translation.z);
//				// ================================================================================================
//				// ImGui - Rotation
//				// ================================================================================================
//				ImGui::TableNextRow();
//				ImGui::Text("Rotation");
//				ImGui::TableNextColumn();
//				ImGui::TableNextColumn();
//				ImGui::TableNextRow();
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("rX", &object->Rotation.x);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("rY", &object->Rotation.y);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("rZ", &object->Rotation.z);
//				// ================================================================================================
//				// ImGui - Scale
//				// ================================================================================================
//				ImGui::TableNextRow();
//				ImGui::Text("Scale");
//				ImGui::TableNextRow();
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("sX", &object->Scale.x);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("sY", &object->Scale.y);
//				ImGui::TableNextColumn();
//				ImGui::InputFloat("sZ", &object->Scale.z);
//				ImGui::EndTable();
//			}
//			ImGui::TreePop();
//		}
//		
//		// ================================================================================================
//		// ImGui - Show transform matrix (Read only)
//		// ================================================================================================
//		if (ImGui::TreeNode("Object Transform (Readonly)")) {
//			if (ImGui::BeginTable("TransformAttributes", 4))
//			{
//				for (int i = 0; i < 4; i++) {
//					for (int j = 0; j < 4; j++) {
//						ImGui::TableNextColumn();
//						ImGui::InputFloat(("m" + std::to_string(i) + std::to_string(j)).c_str(), &object->mTransform[i][j]);
//					}
//
//					ImGui::TableNextRow();
//				}
//				ImGui::EndTable();
//			}
//			ImGui::TreePop();
//		}
//		// ================================================================================================
//		// ImGui - Child Objects
//		// ================================================================================================
//		if (object->GetChildObjects().size() > 0) {
//			if (ImGui::TreeNode("Child Objects")) {
//				for (Object* ChildObject : object->GetChildObjects()) {
//					RenderObjectListTable(ChildObject);
//				}
//				ImGui::TreePop();
//			}
//		}
//
//		// ================================================================================================
//		// ImGui - Child Transforms
//		// ================================================================================================
//		if (object->GetChildTransforms().size() > 0) {
//			if (ImGui::TreeNode("Child Transforms")) {
//				for (Transform* transform : object->GetChildTransforms()) {
//					if (ImGui::TreeNode(transform->GetName().c_str())) {
//						if (ImGui::TreeNode("Transform Attributes")) {
//							if (ImGui::BeginTable("PositionAttributes", 3))
//							{
//								// ================================================================================================
//								// ImGui - Transform - Position
//								// ================================================================================================
//								ImGui::TableNextColumn();
//								ImGui::Text("Position");
//								ImGui::TableNextRow();
//								ImGui::TableNextColumn();
//								ImGui::InputFloat("pX", &transform->Translation.x);
//								ImGui::TableNextColumn();
//								ImGui::InputFloat("pY", &transform->Translation.y);
//								ImGui::TableNextColumn();
//								ImGui::InputFloat("pZ", &transform->Translation.z);
//								// ================================================================================================
//								// ImGui - Transform - Rotation
//								// ================================================================================================
//								ImGui::TableNextRow();
//								ImGui::TableNextColumn();
//								ImGui::Text("Rotation");
//								ImGui::TableNextRow();
//								ImGui::TableNextColumn();
//								ImGui::InputFloat("rX", &transform->Rotation.x);
//								ImGui::TableNextColumn();
//								ImGui::InputFloat("rY", &transform->Rotation.y);
//								ImGui::TableNextColumn();
//								ImGui::InputFloat("rZ", &transform->Rotation.z);
//								// ================================================================================================
//								// ImGui - Transform - Scale
//								// ================================================================================================
//								ImGui::TableNextRow();
//								ImGui::TableNextColumn();
//								ImGui::Text("Scale");
//								ImGui::TableNextRow();
//								ImGui::TableNextColumn();
//								ImGui::InputFloat("sX", &transform->Scale.x);
//								ImGui::TableNextColumn();
//								ImGui::InputFloat("sY", &transform->Scale.y);
//								ImGui::TableNextColumn();
//								ImGui::InputFloat("sZ", &transform->Scale.z);
//								ImGui::EndTable();
//							}
//							ImGui::TreePop();
//						}
//						ImGui::TreePop();
//					}
//				}
//				ImGui::TreePop();
//			}
//		}
//
//		ImGui::TreePop();
//	}
//}