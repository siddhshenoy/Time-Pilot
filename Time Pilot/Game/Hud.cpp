#include "Hud.h"

Shader* Hud::gHudSpriteShader = nullptr;

void Hud::HudBehaviouralFunction(void* param)
{
	Hud* hud = static_cast<Hud*>(param);
	if (hud != nullptr) {
		if (Globals::gIsFighterJetBoosting) {
			if (hud->BoostBox->GetScale().x > 0.0f) {
				hud->BoostBox->SetScale(hud->BoostBox->GetScale() - glm::vec3(100.0f * Globals::gDeltaTime, 0.0f, 0.0f));
				hud->BoostBox->SetColor(glm::vec4(0.75f, 0.10f, 0.10f, 1.0f));
				hud->BoostIcon->SetColor(glm::vec4(0.75f, 0.10f, 0.10f, 1.0f));
			}
			else {
				Globals::gIsFighterJetBoosting = false;
				Globals::gWasFighterJetBoosting = true;
			}
		}
		if (Globals::gWasFighterJetBoosting) {
			if (hud->BoostBox->GetScale().x < hud->OriginalBoostBoxScale.x) {
				hud->BoostBox->SetScale(hud->BoostBox->GetScale() + glm::vec3(100.0f * Globals::gDeltaTime, 0.0f, 0.0f));
				hud->BoostBox->SetColor(glm::vec4(0.05f, 0.60f, 0.10f, 1.0f));
				hud->BoostIcon->SetColor(glm::vec4(0.05f, 0.60f, 0.10f, 1.0f));
			}
			else {
				hud->BoostBox->SetScale(hud->OriginalBoostBoxScale);
				hud->BoostBox->SetColor(hud->OriginalBoostBoxColor);
				hud->BoostIcon->SetColor(hud->OriginalBoostIconColor);
				Globals::gWasFighterJetBoosting = false;
			}
		}

		if (Globals::WasMissileFired) {
			
			if (hud->MissileBox->GetScale().x > (hud->OriginalMissileBoxScale.x * ((float)Globals::CurrentMissiles / (float)Globals::TotalMissiles))) {
				hud->MissileBox->SetScale(hud->MissileBox->GetScale() - glm::vec3(50.0f * Globals::gDeltaTime, 0.0f, 0.0f));
				hud->MissileBox->SetColor(glm::vec4(0.75f, 0.10f, 0.10f, 1.0f));
				hud->MissileIcon->SetColor(glm::vec4(0.75f, 0.10f, 0.10f, 1.0f));
			}
			else {
				Globals::WasMissileFired = false;
			}
			if (hud->MissileBox->GetScale().x <= 0.0f) {
				Globals::WasMissileFired = false;
				Globals::RefillingMissiles = true;
			}
		}
		if (Globals::RefillingMissiles) {
			if (hud->MissileBox->GetScale().x < (hud->OriginalMissileBoxScale.x)) {
				hud->MissileBox->SetScale(hud->MissileBox->GetScale() + glm::vec3(75.0f * Globals::gDeltaTime, 0.0f, 0.0f));
				hud->MissileBox->SetColor(glm::vec4(0.05f, 0.60f, 0.10f, 1.0f));
				hud->MissileIcon->SetColor(glm::vec4(0.05f, 0.60f, 0.10f, 1.0f));
			}
			else {
				hud->MissileBox->SetScale(hud->OriginalMissileBoxScale);
				hud->MissileBox->SetColor(hud->OriginalMissileBoxColor);
				hud->MissileIcon->SetColor(hud->OriginalMssileIconColor);
				Globals::RefillingMissiles = false;
				Globals::CurrentMissiles = Globals::TotalMissiles;
			}
		}
		if (Globals::gHasFighterJetTakenDamage) {
			if (hud->HealthBox->GetScale().x > (hud->OriginalHealthBoxScale.x  * (Globals::gFighterJetHealth / Globals::gFighterJetTotalHealth))) {
				hud->HealthBox->SetScale(hud->HealthBox->GetScale() - glm::vec3(50.0f * Globals::gDeltaTime, 0.0f, 0.0f));
				hud->HealthBox->SetColor(glm::vec4(1.0f, 0.50f, 0.00f, 1.0f));
				hud->HealthIcon->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				hud->HealthBox->SetColor(hud->OriginalHealthBoxColor);
				hud->HealthIcon->SetColor(hud->OriginalHealthIconColor);
				Globals::gHasFighterJetTakenDamage = false;
			}
		}
	}
}

Hud::Hud()
{
	spdlog::info("Creating hud..");
	spdlog::info(std::to_string(Globals::gScreenWidth - 400.0f));
	spdlog::info(std::to_string(Globals::gScreenHeight - 50.0f));
	BoostBoxBackground = new Sprite();
	BoostBoxBackground->LoadSpriteFromPath("./GameResources/Sprites/Hud_Boost/BoostBox_Background.png");
	BoostBoxBackground->SetTranslation(glm::vec3(
		Globals::gScreenWidth - 320.0f,
		Globals::gScreenHeight - 100.0f,
		//300.0f,
		//300.0f,
		0.0f
	));
	BoostBoxBackground->SetScale(glm::vec3(
		300.0f,
		30.0f,
		0.0f
	));

	BoostBox = new Sprite();
	BoostBox->LoadSpriteFromPath("./GameResources/Sprites/Hud_Boost/BoostBox.png");
	BoostBox->SetTranslation(glm::vec3(
		Globals::gScreenWidth - 320.0f,
		Globals::gScreenHeight - 100.0f,
		0.0f
	));
	BoostBox->SetScale(glm::vec3(
		300.0f,
		30.0f,
		0.0f
	));
	BoostBox->SetColor(glm::vec4(0.0f, 0.5f, 0.5f, 1.0f));
	

	BoostIcon = new Sprite();
	BoostIcon->LoadSpriteFromPath("./GameResources/Sprites/Hud_Boost/BoostIcon.png");
	BoostIcon->SetTranslation(glm::vec3(
		Globals::gScreenWidth - 360.0f,
		Globals::gScreenHeight - 100.0f,
		0.0f
	));
	BoostIcon->SetScale(glm::vec3(
		30.0f,
		30.0f,
		0.0f
	));
	BoostIcon->SetColor(glm::vec4(1.0f));

	OriginalBoostBoxScale = BoostBox->GetScale();
	OriginalBoostBoxColor = BoostBox->GetColor();
	OriginalBoostIconColor = BoostIcon->GetColor();

	// ============================================================================================================
	// Missile
	// ============================================================================================================

	MissileBoxBackground = new Sprite();
	MissileBoxBackground->LoadSpriteFromPath("./GameResources/Sprites/Hud_Boost/BoostBox_Background.png");
	MissileBoxBackground->SetTranslation(glm::vec3(
		Globals::gScreenWidth - 320.0f,
		Globals::gScreenHeight - 140.0f,
		//300.0f,
		//300.0f,
		0.0f
	));
	MissileBoxBackground->SetScale(glm::vec3(
		300.0f,
		30.0f,
		0.0f
	));


	MissileBox = new Sprite();
	MissileBox->LoadSpriteFromPath("./GameResources/Sprites/Hud_Boost/BoostBox.png");
	MissileBox->SetTranslation(glm::vec3(
		Globals::gScreenWidth - 320.0f,
		Globals::gScreenHeight - 140.0f,
		0.0f
	));
	MissileBox->SetScale(glm::vec3(
		300.0f,
		30.0f,
		0.0f
	));
	MissileBox->SetColor(glm::vec4(0.0f, 0.5f, 0.5f, 1.0f));

	MissileIcon = new Sprite();
	MissileIcon->LoadSpriteFromPath("./GameResources/Sprites/Hud_Boost/MissileIcon.png");
	MissileIcon->SetTranslation(glm::vec3(
		Globals::gScreenWidth - 360.0f,
		Globals::gScreenHeight - 140.0f,
		0.0f
	));
	MissileIcon->SetScale(glm::vec3(
		30.0f,
		30.0f,
		0.0f
	));
	MissileIcon->SetColor(glm::vec4(1.0f));

	OriginalMissileBoxColor = MissileBox->GetColor();
	OriginalMssileIconColor = MissileIcon->GetColor();
	OriginalMissileBoxScale = MissileBox->GetScale();



	HealthBox = new Sprite();
	HealthBox->LoadSpriteFromPath("./GameResources/Sprites/Hud_Boost/BoostBox.png");
	HealthBox->SetTranslation(glm::vec3(
		Globals::gScreenWidth - 320.0f,
		Globals::gScreenHeight - 180.0f,
		0.0f
	));
	HealthBox->SetScale(glm::vec3(
		300.0f,
		30.0f,
		0.0f
	));
	HealthBox->SetColor(glm::vec4(1.0f, 0.25f, 0.25f, 1.0f));

	HealthBoxBackground = new Sprite();
	HealthBoxBackground->LoadSpriteFromPath("./GameResources/Sprites/Hud_Boost/BoostBox_Background.png");
	HealthBoxBackground->SetTranslation(glm::vec3(
		Globals::gScreenWidth - 320.0f,
		Globals::gScreenHeight - 180.0f,
		//300.0f,
		//300.0f,
		0.0f
	));
	HealthBoxBackground->SetScale(glm::vec3(
		300.0f,
		30.0f,
		0.0f
	));

	HealthIcon = new Sprite();
	HealthIcon->LoadSpriteFromPath("./GameResources/Sprites/Hud_Boost/HealthIcon.png");
	HealthIcon->SetTranslation(glm::vec3(
		Globals::gScreenWidth - 360.0f,
		Globals::gScreenHeight - 180.0f,
		0.0f
	));
	HealthIcon->SetScale(glm::vec3(
		30.0f,
		30.0f,
		0.0f
	));

	OriginalHealthBoxColor = HealthBox->GetColor();
	OriginalHealthIconColor = HealthIcon->GetColor();
	OriginalHealthBoxScale = HealthBox->GetScale();


	if (Hud::gHudSpriteShader != nullptr) {
		BoostIcon->AssignShader(Hud::gHudSpriteShader);
		BoostBoxBackground->AssignShader(Hud::gHudSpriteShader);
		BoostBox->AssignShader(Hud::gHudSpriteShader);

		MissileBoxBackground->AssignShader(Hud::gHudSpriteShader);
		MissileBox->AssignShader(Hud::gHudSpriteShader);
		MissileIcon->AssignShader(Hud::gHudSpriteShader);

		HealthBoxBackground->AssignShader(Hud::gHudSpriteShader);
		HealthBox->AssignShader(Hud::gHudSpriteShader);
		HealthIcon->AssignShader(Hud::gHudSpriteShader);
	}



	this->SetBehaviouralFunction(Hud::HudBehaviouralFunction);
	this->AddBehaviouralFunctionParam(static_cast<void*>(this));
}
void Hud::DrawHud() {
	BoostBox->Render(Globals::GetCameraInstance());
	BoostBoxBackground->Render(Globals::GetCameraInstance());
	BoostIcon->Render(Globals::GetCameraInstance());

	MissileBox->Render(Globals::GetCameraInstance());
	MissileBoxBackground->Render(Globals::GetCameraInstance());
	MissileIcon->Render(Globals::GetCameraInstance());

	HealthBox->Render(Globals::GetCameraInstance());
	HealthBoxBackground->Render(Globals::GetCameraInstance());
	HealthIcon->Render(Globals::GetCameraInstance());
}

Hud::~Hud()
{
	delete BoostBoxBackground;
	delete BoostBox;
	delete BoostIcon;
	delete MissileIcon;
	delete MissileBoxBackground;
	delete MissileBox;
	delete HealthIcon;
	delete HealthBoxBackground;
	delete HealthBox;
}
