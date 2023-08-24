#pragma once

#include <Core/Window.h>

#include <Rendering/Camera.h>
#include <Rendering/Font.h>

#include <Game/Cube.h>

class Globals
{
public:
	static float gDeltaTime;
	static float gGlfwTime;
	static float gProjectionNear;
	static float gProjectionFar;
	static int gFramesPerSecond;
	static int gUpdatedFrames;
	static bool gShouldShowMiniMap;

public:
	static Cube* gFighterJetCube;
	static float gFighterJetOriginalVelocity;
	static float gFighterJetVelocity;
	static float gFighterJetBoostVelocity;
	static float gFighterJetTotalHealth;
	static float gFighterJetHealth;
	static float gFighterPlaneVelocity;
	static bool gFighterPlaneAllRender;
public:
	static bool gIsFighterJetBoosting;
	static bool gWasFighterJetBoosting;
	static unsigned char CurrentMissiles;
	static unsigned char TotalMissiles;
	static bool WasMissileFired;
	static bool RefillingMissiles;
	static bool gHasFighterJetTakenDamage;
	static float HighlightAngle;
	static float HighlightValue;
	static bool IsHighlightOn;
	static bool WasHighlightOn;
public:
	static Camera* gCamera;
	static bool gIsCameraAttachedToFighterJet;
	static float CameraParentZoom;
	static glm::vec3 CameraParentOffset;
	static bool gCameraBoost;
	static float gCameraMoveSpeed;
	static float gCameraOriginalMoveSpeed;
	static float gCameraMoveMultiplier;
public:
	static bool IsGameOver;
	static bool HasGameStarted;
	static bool IsUserInMenu;
	static bool QuitCommand;
public:
	static Window* gWindow;
	static bool gAllowMouseMovement;
	static int gScreenWidth;
	static int gScreenHeight;
public:
	static Font* TextRasterizer;
	static Font* FontRenderer;
public:
	static const float MISSILE_LIFE_IN_SECONDS;
public:
	static void InitializeGlobals();
	static Camera* GetCameraInstance();
};