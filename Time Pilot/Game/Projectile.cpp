#include "Projectile.h"

Material* Projectile::ProjectileMaterial = nullptr;
Texture* Projectile::ProjectileTexture = nullptr;

std::vector<Projectile*> Projectile::ProjectileList;

Projectile::Projectile() {
	this->ProjectileObject = nullptr;
	this->ProjectileForward = nullptr;
	this->IsShot = false;
	Projectile::ProjectileList.push_back(this);
	this->mBaseType = ProjectileBaseType::NONE;
	this->mType = ProjectileOwner::NONE;
}
void Projectile::MissileBehaviour(void* param) {
	Projectile* missile = static_cast<Projectile*>(param);
	if (missile != nullptr) {
		if (missile->IsShot) {
			if (missile->ProjectileObject != nullptr && missile->ProjectileForward != nullptr) {
				if ((Globals::gGlfwTime - missile->InstantiationTime) < Globals::MISSILE_LIFE_IN_SECONDS) {
					missile->ProjectileObject->SetTranslation(
						missile->ProjectileObject->GetTranslation() +
						(missile->ProjectileForward->GetWorldTranslation() - missile->ProjectileObject->GetTranslation()) * missile->GetVelocity() *
						Globals::gDeltaTime
					);
					// =========================================================================================================================
					// Object Collision detection - very basic, rookie style collision detection..
					// I.e., using sphere / circle radius
					// =========================================================================================================================
					if (missile->mType == ProjectileOwner::PROTAGONIST) {
						for (FighterPlane* rust : FighterPlane::FighterPlaneList) {
							if (CommonMath::DistanceBetweenPoints3D(
								missile->ProjectileObject->GetTranslation(),
								rust->FighterPlaneObject->GetTranslation()) < 5.0f) {
								if (missile->mBaseType == ProjectileBaseType::MISSILE) {
									rust->FighterPlaneHealth = 0.0f;
								}
								else if (missile->mBaseType == ProjectileBaseType::BULLET) {
									rust->FighterPlaneHealth -= 0.5f;
								}
								if (rust->FighterPlaneHealth <= 0.0f) {
									std::stringstream ss;
									ss << "Destroying missile on collision [" << (missile->mType == ProjectileOwner::PROTAGONIST ? "Protagonist" : "Enemy") << "] " << missile;
									spdlog::info(ss.str());
									delete rust;
								}
								delete missile;
								break;
							}
						}
					}
					else if(missile->mType == ProjectileOwner::ENEMY) {
						if (FighterJet::gPlayerFighterJet != nullptr) {
							if (CommonMath::DistanceBetweenPoints3D(
								missile->ProjectileObject->GetTranslation(),
								FighterJet::gPlayerFighterJet->FighterJetObject->GetTranslation()) < 5.0f) {
								std::stringstream ss;
								ss << "Destroying missile on collision [" << (missile->mType == ProjectileOwner::PROTAGONIST ? "Protagonist" : "Enemy") << "] " << missile;
								spdlog::info(ss.str());
								Globals::gHasFighterJetTakenDamage = true;
								Globals::gFighterJetHealth -= 20.0f;
								delete missile;
								//Globals::gWindow->CloseWindow();
								
							}
						}
					}

				}
				else {
					//missile->SetBehaviouralFunction(nullptr);
					//Behaviour::RemoveBehaviour(missile);
					std::vector<Projectile*>::iterator location = std::find(Projectile::ProjectileList.begin(), Projectile::ProjectileList.end(), missile);
					Projectile::ProjectileList.erase(location);
					std::stringstream ss;
					ss << "Automatically destroying missile [" << (missile->mType == ProjectileOwner::PROTAGONIST ? "Protagonist" : "Enemy") << "] " << missile;
					spdlog::info(ss.str());
					delete missile;
				}
			}
		}
	}
}
void Projectile::LoadMissileDefaults(const std::string& path) {
	if (Projectile::ProjectileMaterial == nullptr) {
		Projectile::ProjectileMaterial = new Material();
	}
	if (Projectile::ProjectileTexture == nullptr) {
		Projectile::ProjectileTexture = new Texture();
		Projectile::ProjectileTexture->LoadTexture((path + "/Missile_texture.png").c_str());
	}
	Projectile::ProjectileMaterial->AttachTexture(Projectile::ProjectileTexture);
}
void Projectile::Load(const std::string& path, const std::string& model_name) {
	std::stringstream ss;
	ss << "Creating missile " << this;
	//spdlog::info(ss.str());
	InstantiationTime = Globals::gGlfwTime;

	std::string load_model_name = "Missile.fbx";
	if (model_name != "")
		load_model_name = model_name;

	this->ProjectileObject = ResourceManager::LoadObject(path + "/" + load_model_name);
	this->ProjectileObject->AssignMaterial(Projectile::ProjectileMaterial);
	
	this->ProjectileForward = new Transform();
	this->ProjectileForward->SetTranslation(glm::vec3(0.0f, 0.0f, -1.0f));

	this->ProjectileObject->AddChildTransform(this->ProjectileForward);

	this->SetBehaviouralFunction(Projectile::MissileBehaviour);
	this->AddBehaviouralFunctionParam(static_cast<void*>(this));
}
void Projectile::Shoot() {
	this->IsShot = true;
}
void Projectile::SetMissileType(ProjectileOwner type) {

	mType = type;
}
Projectile::~Projectile() {
	ResourceManager::DestroyObject(ProjectileObject);
	delete ProjectileForward;
	ProjectileForward = nullptr;
	delete ProjectileObject;
	ProjectileObject = nullptr;
}