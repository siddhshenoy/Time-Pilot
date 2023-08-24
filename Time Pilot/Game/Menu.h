#pragma once

#include <Rendering/Font.h>
#include <Core/Window.h>

#include <Rendering/Sprite.h>

#include <Game/Globals.h>
#include <Game/Text2D.h>

#include <vector>

enum class MenuScenes {
	MENU_MAIN,
	MENU_ABOUT
};

class Menu
{
public:
	static Font* MenuFont;
	static Shader* gMenuSpriteShader;
public:
	MenuScenes MenuScene;
	Sprite* Menu_Logo;
	Text2D* MenuItem_Heading;
	Text2D* MenuItem_Play;
	Text2D* MenuItem_About;
	Text2D* MenuItem_AboutText;
	Text2D* MenuItem_Back;
	Text2D* MenuItem_Quit;
	std::vector<std::string> AboutStringList;
public:
	static void MenuItem_Play_OnClick(std::vector<void*> param);
	static void MenuItem_Quit_OnClick(std::vector<void*> param);
	static void MenuItem_About_OnClick(std::vector<void*> param);
	static void MenuItem_Back_OnClick(std::vector<void*> param);
public:
	Menu();
	void Draw();
	~Menu();
};

