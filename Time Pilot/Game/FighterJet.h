#pragma once

#include <Resource/ResourceManager.h>
#include <Rendering/Object.h>
#include <Rendering/Transform.h>

#include <Game/Globals.h>
#include <Game/Behaviour.h>
#include <Game/Moveable.h>
#include <Game/ProjectileDummy.h>
#include <Game/Projectile.h>

class FighterJet : public Behaviour, public Moveable
{
public:
	Object* FighterJetObject;
	//Object* HydraDoor;
	//Object* HydraDebugCube;
	ProjectileDummy* FighterJetMissiles[2];
	ProjectileDummy* FighterJetBullets[2];
	Transform* FighterJetForward;
	float LastBulletShotTime;
	float BulletFiringRate;
public:
	// ====================================================================================================================
	// Material & Texture for FighterJet
	// ====================================================================================================================
	static Texture* HydraTexture;
	static Material* HydraMaterial;
	static FighterJet* gPlayerFighterJet;
public:
	static void LoadFighterJetDefaults(const std::string& path);
	static void HydraBehaviouralFunction(void* param);
public:
	FighterJet();
	void Load(const std::string& path);
	void ShootMissile();
	void ShootBullet();
	~FighterJet();

};

