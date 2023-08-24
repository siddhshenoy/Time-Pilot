#pragma once

#include <Game/Behaviour.h>
#include <Game/Globals.h>

#include <Rendering/Sprite.h>

class Hud : public Behaviour
{
public:
	static Shader* gHudSpriteShader;
public:
	static void HudBehaviouralFunction(void* param);
public:
	Sprite* BoostIcon;
	Sprite* BoostBoxBackground;
	Sprite* BoostBox;
	glm::vec3 OriginalBoostBoxScale;
	glm::vec4 OriginalBoostBoxColor;
	glm::vec4 OriginalBoostIconColor;

	Sprite* MissileIcon;
	Sprite* MissileBoxBackground;
	Sprite* MissileBox;
	glm::vec3 OriginalMissileBoxScale;
	glm::vec4 OriginalMissileBoxColor;
	glm::vec4 OriginalMssileIconColor;

	Sprite* HealthIcon;
	Sprite* HealthBoxBackground;
	Sprite* HealthBox;
	glm::vec3 OriginalHealthBoxScale;
	glm::vec4 OriginalHealthBoxColor;
	glm::vec4 OriginalHealthIconColor;;

public:
	Hud();
	void DrawHud();
	~Hud();
};

