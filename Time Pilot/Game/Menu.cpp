#include "Menu.h"

Font* Menu::MenuFont = nullptr;
Shader* Menu::gMenuSpriteShader = nullptr;

Menu::Menu() {
	Menu::MenuFont = new Font();
	Menu::MenuFont->LoadFont("./GameResources/Fonts/cs_regular.ttf");

	this->MenuScene = MenuScenes::MENU_MAIN;

	Menu_Logo = new Sprite();
	Menu_Logo->LoadSpriteFromPath("./GameResources/Sprites/Menu/Logo.png");
	Menu_Logo->SetTranslation(glm::vec3(
		Globals::gScreenWidth/2 + 200,
		Globals::gScreenHeight/2 - 200,
		0.0f
	));
	Menu_Logo->SetScale(glm::vec3(
		200.0f,
		200.0f,
		0.0f
	));
	if(Menu::gMenuSpriteShader != nullptr)
		Menu_Logo->AssignShader(Menu::gMenuSpriteShader);

	MenuItem_Heading = new Text2D();
	MenuItem_Heading->SetText("Time Pilot");
	MenuItem_Heading->Draw();
	MenuItem_Heading->SetScale(1.65f);
	MenuItem_Heading->SetColor(glm::vec3(1.0f, 0.343f, 0.0f));
	MenuItem_Heading->SetHoverable(false);

	MenuItem_Play = new Text2D();
	MenuItem_Play->SetText("Play");
	MenuItem_Play->Draw();
	MenuItem_Play->SetScale(1.0f);
	MenuItem_Play->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	MenuItem_Play->SetHoverable(true);
	MenuItem_Play->SetHoverColor(glm::vec3(0.0f, 1.0f, 0.0f));
	MenuItem_Play->SetOnClickFunction(MenuItem_Play_OnClick);

	MenuItem_About = new Text2D();
	MenuItem_About->SetText("About");
	MenuItem_About->SetScale(1.0f);
	MenuItem_About->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	MenuItem_About->SetHoverable(true);
	MenuItem_About->SetHoverColor(glm::vec3(0.0f, 1.0f, 0.0f));
	MenuItem_About->AddOnClickParam(static_cast<void*>(this));
	MenuItem_About->SetOnClickFunction(MenuItem_About_OnClick);

	MenuItem_AboutText = new Text2D();
	MenuItem_AboutText->SetText("");
	MenuItem_AboutText->SetScale(0.5f);
	MenuItem_AboutText->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	MenuItem_AboutText->SetHoverable(false);


	MenuItem_Back = new Text2D();
	MenuItem_Back->SetText("Back");
	MenuItem_Back->Draw();
	MenuItem_Back->SetScale(1.0f);
	MenuItem_Back->SetColor(glm::vec3(0.5f, 0.5f, 0.5f));
	MenuItem_Back->SetHoverable(true);
	MenuItem_Back->AddOnClickParam(static_cast<void*>(this));
	MenuItem_Back->SetHoverColor(glm::vec3(0.0f, 1.0f, 0.0f));
	MenuItem_Back->SetOnClickFunction(MenuItem_Back_OnClick);


	MenuItem_Quit = new Text2D();
	MenuItem_Quit->SetText("Quit");
	MenuItem_Quit->Draw();
	MenuItem_Quit->SetScale(1.0f);
	MenuItem_Quit->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	MenuItem_Quit->SetHoverable(true);
	MenuItem_Quit->SetHoverColor(glm::vec3(0.0f, 1.0f, 0.0f));
	MenuItem_Quit->SetOnClickFunction(MenuItem_Quit_OnClick);

	AboutStringList.push_back("Time Pilot v0.1a");
	AboutStringList.push_back("Plane Controls:");
	AboutStringList.push_back("1. Use W, A, S, D to rotate the plane.");
	AboutStringList.push_back("2. Press B to boost the plane.");
	AboutStringList.push_back("3. Press LEFT CTRL to shoot missiles and hold RIGHT CTRL to shoot bullets (Use LMB / RMB as alternatives).");
	AboutStringList.push_back("");
	AboutStringList.push_back("Camera Controls:");
	AboutStringList.push_back("1. Press U to unlock the camera from the plane.");
	AboutStringList.push_back("2. Press C to enable the cursor.");
	AboutStringList.push_back("3. Use scrollbar to zoom in/out when camera is attached to the plane.");
	AboutStringList.push_back("");
	AboutStringList.push_back("Debugging Controls:");
	AboutStringList.push_back("1. Press T to toggle ImGUI debugging window");
	AboutStringList.push_back("");
	AboutStringList.push_back("Developer: Siddharth Shenoy");
}
void Menu::MenuItem_Back_OnClick(std::vector<void*> param)
{
	if (param.size() >= 2) {
		Menu* menu = static_cast<Menu*>(param[1]);
		if (menu != nullptr) {
			if(menu->MenuScene == MenuScenes::MENU_ABOUT)
				menu->MenuScene = MenuScenes::MENU_MAIN;
		}
	}
}
void Menu::MenuItem_Quit_OnClick(std::vector<void*> param) {
	spdlog::info("quitting game");
	Globals::gWindow->CloseWindow();
}

void Menu::Draw() {
	
	Font* OldFontRenderer = Globals::FontRenderer;
	Globals::FontRenderer = MenuFont;
	MenuItem_Heading->SetHasDrawn(false);
	MenuItem_Play->SetHasDrawn(false);
	MenuItem_About->SetHasDrawn(false);
	MenuItem_Quit->SetHasDrawn(false);
	MenuItem_AboutText->SetHasDrawn(false);
	MenuItem_Back->SetHasDrawn(false);
	//Draw menu
	if (!Globals::IsGameOver) {
		if (this->MenuScene == MenuScenes::MENU_MAIN) {

			MenuItem_Heading->Draw();
			MenuItem_Heading->SetTranslation(glm::vec2(
				(Globals::gScreenWidth - MenuItem_Heading->GetTextDimensions().x) / 2,
				((Globals::gScreenHeight - MenuItem_Heading->GetTextDimensions().y) / 2) + 70.0f));

			if (Globals::HasGameStarted) {
				MenuItem_Play->SetText("Resume");
			}
			MenuItem_Play->Draw();
			MenuItem_Play->SetTranslation(glm::vec2(
				(Globals::gScreenWidth - MenuItem_Play->GetTextDimensions().x) / 2,
				(Globals::gScreenHeight - MenuItem_Play->GetTextDimensions().y) / 2));

			MenuItem_About->Draw();
			MenuItem_About->SetTranslation(glm::vec2(
				(Globals::gScreenWidth - MenuItem_About->GetTextDimensions().x) / 2,
				((Globals::gScreenHeight - MenuItem_About->GetTextDimensions().y) / 2) - 50.0f));

			MenuItem_Quit->Draw();
			MenuItem_Quit->SetTranslation(glm::vec2(
				(Globals::gScreenWidth - MenuItem_Quit->GetTextDimensions().x) / 2,
				((Globals::gScreenHeight - MenuItem_Quit->GetTextDimensions().y) / 2) - 100.0f));

			Menu_Logo->Render(Globals::GetCameraInstance());

			MenuItem_Heading->SetHasDrawn(true);
			MenuItem_Play->SetHasDrawn(true);
			MenuItem_About->SetHasDrawn(true);
			MenuItem_Quit->SetHasDrawn(true);
		}
		else if (this->MenuScene == MenuScenes::MENU_ABOUT) {
			Globals::FontRenderer = OldFontRenderer;
			int i = 0;
			for (i = 0; i < this->AboutStringList.size(); i++) {
				MenuItem_AboutText->SetText(this->AboutStringList[i]);
				MenuItem_AboutText->SetTranslation(glm::vec2(
					(Globals::gScreenWidth) / 2 - 600.0f,
					((Globals::gScreenHeight) / 2) + 100.0f - (25.0f * i))
				);

				MenuItem_AboutText->Draw();
			}
			i++;
			Globals::FontRenderer = MenuFont;
			MenuItem_Back->Draw();
			MenuItem_Back->SetTranslation(glm::vec2(
				(Globals::gScreenWidth - MenuItem_Back->GetTextDimensions().x) / 2,
				((Globals::gScreenHeight - MenuItem_Back->GetTextDimensions().y) / 2) + 100.0f - 25 * i));
			MenuItem_Back->Draw();
			MenuItem_AboutText->SetHasDrawn(true);
			MenuItem_Back->SetHasDrawn(true);
		}
	}
	else {
		MenuItem_Heading->SetText("Game Over!");
		MenuItem_Heading->Draw();
		MenuItem_Heading->SetTranslation(glm::vec2(
			(Globals::gScreenWidth - MenuItem_Heading->GetTextDimensions().x) / 2,
			((Globals::gScreenHeight - MenuItem_Heading->GetTextDimensions().y) / 2) + 70.0f));
		MenuItem_Quit->SetTranslation(glm::vec2(
			(Globals::gScreenWidth - MenuItem_Quit->GetTextDimensions().x) / 2,
			((Globals::gScreenHeight - MenuItem_Quit->GetTextDimensions().y) / 2)));
		MenuItem_Quit->Draw();
		MenuItem_Heading->SetHasDrawn(true);
		MenuItem_Quit->SetHasDrawn(true);

	}
	//Switch back to old font
	Globals::FontRenderer = OldFontRenderer;

}
void Menu::MenuItem_Play_OnClick(std::vector<void*> params) {
	Globals::IsUserInMenu = false;
	Globals::gAllowMouseMovement = true;
	Globals::HasGameStarted = true;
	Globals::gWindow->HideCursor();
}
void Menu::MenuItem_About_OnClick(std::vector<void*> param)
{
	if (param.size() >= 2) {
		Menu* menu = static_cast<Menu*>(param[1]);
		if (menu != nullptr) {
			menu->MenuScene = MenuScenes::MENU_ABOUT;
		}
	}
}
Menu::~Menu() {
}