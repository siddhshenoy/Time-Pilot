#include "FighterPlane.h"

Material* FighterPlane::FighterPlaneMaterial = nullptr;
Texture* FighterPlane::FighterPlaneTexture = nullptr;
float FighterPlane::FighterJetRepositioningTime = Globals::gGlfwTime;
std::vector<FighterPlane*> FighterPlane::FighterPlaneList;

FighterPlane::FighterPlane()
{
	this->FighterPlaneHealth = 100.0f;
	FighterPlane::FighterPlaneList.push_back(this);
}
bool FighterPlane::FighterPlanePreRenderingFunction(Object* object) {
	if (!Globals::gFighterPlaneAllRender) {
		bool does_render = Globals::gFighterJetCube->DoesPointFallInCube(object->GetTranslation());
		return does_render;
	}
	return true;
	//return true;
}

void FighterPlane::LoadFighterPlaneDefaults(const std::string& path) {

	if (FighterPlaneMaterial == nullptr) {
		FighterPlaneMaterial = new Material();
	}
	if (FighterPlaneTexture == nullptr) {
		FighterPlaneTexture = new Texture();
		FighterPlaneTexture->LoadTexture((path + "/FighterPlane_Texture.png").c_str());
		FighterPlaneMaterial->AttachTexture(FighterPlaneTexture);
	}
}
void FighterPlane::Load(const std::string& path)
{
	this->LastBulletShotTime = Globals::gGlfwTime;
	this->FighterPlaneObject = ResourceManager::LoadObject(path + "/FighterPlane.fbx");
	this->FighterPlaneObject->SetScale(glm::vec3(0.01f));
	this->FighterPlaneObject->SetTranslation(glm::vec3(0.0f));
	this->FighterPlaneObject->AssignMaterial(FighterPlaneMaterial);
	// Attach the forward transform 
	this->FighterPlaneForward = new Transform();
	this->FighterPlaneForward->SetName("FighterPlane Forward Vector");
	this->FighterPlaneForward->SetTranslation(glm::vec3(0.0f, 0.0f, -100.0f));
	this->FighterPlaneObject->AddChildTransform(this->FighterPlaneForward);

	// Rotor for aesthetics
	this->FighterPlaneRotor = ResourceManager::LoadObject(path + "/FighterPlane_Blades.fbx");
	this->FighterPlaneRotor->SetTranslation(glm::vec3(0.014771, -0.172975, -6.42772) * 100.0f);
	this->FighterPlaneRotor->AssignMaterial(FighterPlaneMaterial);

	this->FighterPlaneObject->AddChildObject(this->FighterPlaneRotor);
	this->SetBehaviouralFunction(FighterPlane::FighterPlaneBehaviouralFunction);
	this->AddBehaviouralFunctionParam(static_cast<void*>(this));
	
	//Create 2 bullets and attach it to the rustler plane
	for (int i = 0; i < sizeof(FighterPlaneBullets) / sizeof(ProjectileDummy); i++) {
		this->FighterPlaneBullets[i] = new ProjectileDummy();
		this->FighterPlaneBullets[i]->Load("./GameResources/Models/EnemyPlanes/FighterPlane","FighterPlane_Missile.fbx");
		
		this->FighterPlaneBullets[i]->ProjectileDummyObject->SetRotation(glm::vec3(0.0f, 0.0f, 45.0f));
		this->FighterPlaneBullets[i]->ProjectileDummyObject->SetScale(glm::vec3(0.01f));
		this->FighterPlaneObject->AddChildObject(this->FighterPlaneBullets[i]->ProjectileDummyObject);
	}
	this->FighterPlaneBullets[0]->ProjectileDummyObject->SetTranslation(glm::vec3(1.85, -0.83, -0.64536) * 100.0f);
	this->FighterPlaneBullets[1]->ProjectileDummyObject->SetTranslation(glm::vec3(-1.85, -0.83, -0.64536) * 100.0f);

	float rX = 0.0f, rZ = 0.0f;
	bool isProper = true;
	do {
		rX = 0.0f;
		if (Utils::GenerateRandom(0, 1) == 0) {
			rX = Utils::GenerateRandomFloat(0, 500);
		}
		else {
			rX = -Utils::GenerateRandomFloat(0, 500);
		}
		rZ = -Utils::GenerateRandomFloat(50, 1000);
	} while (!isProper);
	this->FighterPlaneObject->SetTranslation(glm::vec3(rX, 0.0f, rZ));
	this->FighterPlaneObject->SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));
	this->FighterPlaneObject->mPreRenderingFunction = FighterPlane::FighterPlanePreRenderingFunction;
	this->FighterPlaneObject->KeyList.Set("LastRepositioningTime", Globals::gGlfwTime);

}
void FighterPlane::RemoveFighterPlaneFromList(FighterPlane* rustler) {
	std::vector<FighterPlane*>::iterator loc = std::find(FighterPlane::FighterPlaneList.begin(), FighterPlane::FighterPlaneList.end(), rustler);
	FighterPlane::FighterPlaneList.erase(loc);
}
void FighterPlane::FighterPlaneBehaviouralFunction(void* param) {
	FighterPlane* pFighterPlane = static_cast<FighterPlane*>(param);
	if (pFighterPlane != nullptr) {
		glm::vec3 FighterPlanePos = pFighterPlane->FighterPlaneObject->GetTranslation();
		glm::vec3 FighterPlaneForwardPos = pFighterPlane->FighterPlaneForward->GetWorldTranslation();
		pFighterPlane->FighterPlaneObject->SetTranslation(
			FighterPlanePos
			+
			(FighterPlaneForwardPos - FighterPlanePos) * pFighterPlane->GetVelocity() * Globals::gDeltaTime
		);
		pFighterPlane->FighterPlaneRotor->SetRotation(pFighterPlane->FighterPlaneRotor->GetRotation() + (glm::vec3(0.0f, 0.0f, 500.0f) * Globals::gDeltaTime));
		if (CommonMath::DistanceBetweenPoints3D(pFighterPlane->FighterPlaneObject->GetTranslation(), FighterJet::gPlayerFighterJet->FighterJetObject->GetTranslation()) < 3.0f && !Globals::gHasFighterJetTakenDamage) {
			Globals::gFighterJetHealth -= 20.0f;
			Globals::gHasFighterJetTakenDamage = true;
		}
		if (Globals::gFighterJetCube->DoesPointFallInCube(pFighterPlane->FighterPlaneObject->GetTranslation())) {
			if ((Globals::gGlfwTime - pFighterPlane->LastBulletShotTime) > 10.0f) {
				//We create some missiles as AI
				for (ProjectileDummy* bullet : pFighterPlane->FighterPlaneBullets) {
					Projectile* missile = new Projectile();
					missile->Load("./GameResources/Models/Missile");
					missile->SetMissileType(ProjectileOwner::ENEMY);
					missile->ProjectileObject->SetTranslation(bullet->ProjectileDummyObject->GetWorldTranslation());
					glm::vec3 wrot = bullet->ProjectileDummyObject->GetWorldRotation();
					missile->ProjectileObject->SetRotation(glm::vec3(-wrot.x, wrot.y, wrot.z));
					missile->SetVelocity(6000.0f);
					missile->ProjectileObject->SetScale(glm::vec3(0.01f));
					missile->Shoot();

				}
				pFighterPlane->LastBulletShotTime = Globals::gGlfwTime;
			}
		}

	}
}
FighterPlane::~FighterPlane()
{
	FighterPlane::RemoveFighterPlaneFromList(this);
	ResourceManager::DestroyObject(this->FighterPlaneObject);
	ResourceManager::DestroyObject(this->FighterPlaneRotor);
	for (ProjectileDummy* bul : FighterPlaneBullets) {
		delete bul;
	}
	delete this->FighterPlaneObject;
	delete this->FighterPlaneRotor;
}
