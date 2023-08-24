#include "Globals.h"

float Globals::gDeltaTime = 0.0f;
float Globals::gGlfwTime = 0.0f;
float Globals::gProjectionNear = 0.001f;
float Globals::gProjectionFar = 1000.0f;

Font* Globals::TextRasterizer = nullptr;

int Globals::gFramesPerSecond = 0;
int Globals::gUpdatedFrames = 0;
bool Globals::gShouldShowMiniMap = false;
bool Globals::WasHighlightOn = false;
Cube* Globals::gFighterJetCube = nullptr;

float Globals::gFighterJetVelocity = 10.0f;
float Globals::gFighterJetOriginalVelocity = Globals::gFighterJetVelocity;
float Globals::gFighterJetBoostVelocity = 50.0f;
float Globals::gFighterPlaneVelocity = 7.0f;
bool Globals::gFighterPlaneAllRender = false; // Is a debugging variable
float Globals::HighlightAngle = 0.0f;
float Globals::HighlightValue = 0.0f;
bool Globals::IsHighlightOn = false;

float Globals::gFighterJetTotalHealth = 100.0f;
float Globals::gFighterJetHealth = 100.0f;

bool Globals::gIsFighterJetBoosting = false;
bool Globals::gWasFighterJetBoosting = false;
unsigned char Globals::CurrentMissiles = 5;
unsigned char Globals::TotalMissiles = 5;
bool Globals::WasMissileFired = false;
bool Globals::RefillingMissiles = false;
bool Globals::gHasFighterJetTakenDamage = false;


Camera* Globals::gCamera = nullptr;
bool Globals::gIsCameraAttachedToFighterJet = true;
glm::vec3 Globals::CameraParentOffset = glm::vec3(0.0f);
float Globals::CameraParentZoom = 25.0f;
bool Globals::gCameraBoost = false;
float Globals::gCameraMoveSpeed = 30.0f;
float Globals::gCameraOriginalMoveSpeed = Globals::gCameraMoveSpeed;
float Globals::gCameraMoveMultiplier = 3.33f;

Window* Globals::gWindow = nullptr;
bool Globals::gAllowMouseMovement = true;
int Globals::gScreenWidth;
int Globals::gScreenHeight;
Font* Globals::FontRenderer = nullptr;

bool Globals::IsGameOver = false;
bool Globals::HasGameStarted = false;
bool Globals::IsUserInMenu = true;
bool Globals::QuitCommand = false;

const float Globals::MISSILE_LIFE_IN_SECONDS = 3.0f; // 5 Seconds

void Globals::InitializeGlobals()
{
	Globals::gCamera = new Camera();
}

Camera* Globals::GetCameraInstance()
{
	return Globals::gCamera;
}
