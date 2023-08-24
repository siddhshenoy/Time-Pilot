#include "FighterJet.h"

Texture* FighterJet::HydraTexture = nullptr;
Material* FighterJet::HydraMaterial = nullptr;
FighterJet* FighterJet::gPlayerFighterJet = nullptr;

void FighterJet::LoadFighterJetDefaults(const std::string& path) {
	if (FighterJet::HydraMaterial == nullptr) {
		FighterJet::HydraMaterial = new Material();
	}
	if (FighterJet::HydraTexture == nullptr) {
		FighterJet::HydraTexture = new Texture();
		FighterJet::HydraTexture->LoadTexture((path + "/FighterJet_texture.png").c_str());
		FighterJet::HydraMaterial->AttachTexture(FighterJet::HydraTexture);
	}

}
void FighterJet::HydraBehaviouralFunction(void* param)
{
	FighterJet* hydra = static_cast<FighterJet*>(param);
	if (hydra != nullptr)
	{
		glm::vec3 forward = hydra->FighterJetForward->GetWorldTranslation();
		glm::vec3 diff = (forward - hydra->FighterJetObject->GetTranslation());
		hydra->FighterJetObject->SetTranslation(hydra->FighterJetObject->GetTranslation() + (forward - hydra->FighterJetObject->GetTranslation()) * hydra->GetVelocity() * Globals::gDeltaTime);
		Globals::gFighterJetCube->SetCentre(hydra->FighterJetObject->GetTranslation());
		if (Globals::gIsCameraAttachedToFighterJet)
			Globals::GetCameraInstance()->LookAt(hydra->FighterJetObject->GetTranslation() + (Globals::CameraParentOffset * Globals::CameraParentZoom), hydra->FighterJetObject->GetTranslation(), glm::vec3(0.0f, 1.0f, 0.0f));
		if (Globals::gFighterJetHealth <= 0.0f) {
			Globals::IsUserInMenu = true;
			Globals::IsGameOver = true;
			Globals::gWindow->ShowCursor();
		}
	}
}

FighterJet::FighterJet()
{
	FighterJet::gPlayerFighterJet = this;
	this->LastBulletShotTime = Globals::gGlfwTime;
	this->BulletFiringRate = 20; // 20 Bullets per second
}

void FighterJet::Load(const std::string& path)
{
	this->FighterJetObject = ResourceManager::LoadObject(path + "/FighterJet.fbx");
	this->FighterJetObject->AssignMaterial(FighterJet::HydraMaterial);
	this->FighterJetObject->SetScale(glm::vec3(0.01f));

	/*this->HydraDoor = ResourceManager::LoadObject(path + "/Hydra_door.fbx", 0.35F);
	this->HydraDoor->AssignMaterial(Hydra::HydraMaterial);
	this->HydraDoor->SetTranslation(glm::vec3(0.0f, 76.484f, -254.456f));*/

	//this->HydraDebugCube = ResourceManager::LoadObject("./Models/Cube.fbx");
	//this->HydraDebugCube->AssignMaterial(Hydra::HydraMaterial);
	//this->HydraDebugCube->SetScale(glm::vec3(0.01f));

	this->FighterJetMissiles[0] = new ProjectileDummy();
	this->FighterJetMissiles[0]->Load("./GameResources/Models/FighterJet", "FighterJet_Missile.fbx");
	//this->HydraBullet[0]->BulletObject->SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));
	this->FighterJetMissiles[0]->ProjectileDummyObject->SetTranslation(glm::vec3(4.85959, -0.410208, 1.975) * 100.0f);

	this->FighterJetMissiles[1] = new ProjectileDummy();
	this->FighterJetMissiles[1]->Load("./GameResources/Models/FighterJet", "FighterJet_Missile.fbx");
	//this->HydraBullet[1]->BulletObject->SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));
	//this->HydraMissiles[1]->BulletObject->SetTranslation(glm::vec3(-3.848, -0.5719, 0.8446) * 100.0f);
	this->FighterJetMissiles[1]->ProjectileDummyObject->SetTranslation(glm::vec3(-4.85959, -0.410208, 1.975) * 100.0f);

	this->FighterJetBullets[0] = new ProjectileDummy();
	this->FighterJetBullets[0]->Load("./GameResources/Models/FighterJet", "FighterJet_Bullet.fbx");
	this->FighterJetBullets[0]->ProjectileDummyObject->SetTranslation(glm::vec3(-3.52665, -0.360309, 1.6936) * 100.0f);

	this->FighterJetBullets[1] = new ProjectileDummy();
	this->FighterJetBullets[1]->Load("./GameResources/Models/FighterJet", "FighterJet_Bullet.fbx");
	this->FighterJetBullets[1]->ProjectileDummyObject->SetTranslation(glm::vec3(3.52665, -0.360309, 1.69363) * 100.0f);

	this->FighterJetForward = new Transform();
	this->FighterJetForward->SetTranslation(glm::vec3(0.0f, 0.0f, -100.0f));
	this->FighterJetForward->SetName("Hydra Forward Vector");
	//this->HydraObject->AddChildObject(this->HydraDoor);
	this->FighterJetObject->AddChildObject(this->FighterJetMissiles[0]->ProjectileDummyObject);
	this->FighterJetObject->AddChildObject(this->FighterJetMissiles[1]->ProjectileDummyObject);
	this->FighterJetObject->AddChildObject(this->FighterJetBullets[0]->ProjectileDummyObject);
	this->FighterJetObject->AddChildObject(this->FighterJetBullets[1]->ProjectileDummyObject);
	this->FighterJetObject->AddChildTransform(FighterJetForward);

	this->SetBehaviouralFunction(FighterJet::HydraBehaviouralFunction);
	this->AddBehaviouralFunctionParam(static_cast<void*>(this));
}
void FighterJet::ShootMissile()
{
	for (int i = 0; i < 2; i++) {
		Projectile* missile = new Projectile();
		missile->Load("./GameResources/Models/FighterJet", "FighterJet_Missile.fbx");
		missile->ProjectileObject->SetTranslation(this->FighterJetMissiles[i]->ProjectileDummyObject->GetWorldTranslation());
		glm::vec3 wrot = this->FighterJetMissiles[i]->ProjectileDummyObject->GetWorldRotation();
		missile->ProjectileForward->SetTranslation(glm::vec3(0.0f, 0.0f, -100.0f));
		missile->ProjectileObject->SetRotation(glm::vec3(wrot.x, wrot.y, wrot.z));
		missile->SetVelocity(60.0f);
		missile->ProjectileObject->SetScale(glm::vec3(0.01f));
		missile->SetMissileType(ProjectileOwner::PROTAGONIST);
		missile->mBaseType = ProjectileBaseType::MISSILE;
		missile->Shoot();
	}
}
void FighterJet::ShootBullet()
{
	for (int i = 0; i < 2; i++) {
		Projectile* missile = new Projectile();
		missile->Load("./GameResources/Models/FighterJet", "FighterJet_Missile.fbx");
		missile->ProjectileObject->SetTranslation(this->FighterJetBullets[i]->ProjectileDummyObject->GetWorldTranslation());
		glm::vec3 wrot = this->FighterJetBullets[i]->ProjectileDummyObject->GetWorldRotation();
		missile->ProjectileForward->SetTranslation(glm::vec3(0.0f, 0.0f, -100.0f));
		missile->ProjectileObject->SetRotation(glm::vec3(wrot.x, wrot.y, wrot.z));
		missile->SetVelocity(90.0f);
		missile->ProjectileObject->SetScale(glm::vec3(0.01f, 0.01f, 0.005f));
		missile->SetMissileType(ProjectileOwner::PROTAGONIST);
		missile->mBaseType = ProjectileBaseType::BULLET;
		missile->Shoot();
	}
}
FighterJet::~FighterJet()
{
	ResourceManager::DestroyObject(this->FighterJetObject);
	//ResourceManager::DestroyObject(this->HydraDoor);
	delete this->FighterJetObject;
	//delete this->HydraDoor;
	delete this->FighterJetMissiles[0];
	delete this->FighterJetMissiles[1];
	delete this->FighterJetBullets[0];
	delete this->FighterJetBullets[1];
}
