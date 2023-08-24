#pragma once

#include <sstream>
#include <vector>

#include <Rendering/Object.h>

#include <Game/Moveable.h>
#include <Game/Behaviour.h>

#include <Game/FighterPlane.h>
#include <Game/FighterJet.h>
#include <Math/CommonMath.h>

enum class ProjectileBaseType {
	NONE,
	MISSILE,
	BULLET
};
enum class ProjectileOwner {
	NONE,
	PROTAGONIST,
	ENEMY
};

class Projectile : public Behaviour , public Moveable
{
public:
	float InstantiationTime;
	Object* ProjectileObject;
	Transform* ProjectileForward;
	ProjectileBaseType mBaseType;
	ProjectileOwner mType;
	bool IsShot;
public:
	static Material* ProjectileMaterial;
	static Texture* ProjectileTexture;
	static std::vector<Projectile*> ProjectileList;
public:
	static void MissileBehaviour(void* Missile);
	static void LoadMissileDefaults(const std::string& path);
	static void CleanupMissileObjectsOnScene() {
		for (Projectile* mis : Projectile::ProjectileList) {
			if (Behaviour::BehaviourInMemory.find(mis) != Behaviour::BehaviourInMemory.end()) {
				if (Behaviour::BehaviourInMemory[mis] == false) {
					std::stringstream ss;
					ss << "Cleaning up missile object on address: " << mis;
					spdlog::warn(ss.str());
					std::vector<Projectile*>::iterator it = std::find(Projectile::ProjectileList.begin(), Projectile::ProjectileList.end(), mis);
					if (it != Projectile::ProjectileList.end()) {
						Projectile::ProjectileList.erase(it);
					}
					ResourceManager::DestroyObject(mis->ProjectileObject);
					Behaviour::BehaviourInMemory.erase(mis);

				}
			}
		}
	}
public:
	Projectile();
	void Load(const std::string& path, const std::string& model_name = "");
	void SetMissileType(ProjectileOwner type);
	void Shoot();
	~Projectile();
};

