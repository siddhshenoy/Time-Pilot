#pragma once

#include <Resource/ResourceManager.h>
#include <Rendering/Object.h>
#include <Rendering/Transform.h>


#include <Game/Globals.h>
#include <Game/Behaviour.h>
#include <Game/Moveable.h>
#include <Game/FighterJet.h>
#include <Game/Projectile.h>
#include <Game/ProjectileDummy.h>

class FighterPlane : public Behaviour, public Moveable
{
public:
	Object* FighterPlaneObject;
	Object* FighterPlaneRotor;

	ProjectileDummy* FighterPlaneBullets[2];

	Transform* FighterPlaneForward;

	float LastBulletShotTime;
	float FighterPlaneHealth;
public:
	static float FighterJetRepositioningTime;
	static Texture* FighterPlaneTexture;
	static Material* FighterPlaneMaterial;
	static std::vector<FighterPlane*> FighterPlaneList;
public:
	static void RemoveFighterPlaneFromList(FighterPlane* rustler);
public:
	static void LoadFighterPlaneDefaults(const std::string& path);
	static bool FighterPlanePreRenderingFunction(Object* object);
	static void FighterPlaneBehaviouralFunction(void* param);
public:
	FighterPlane();
	void Load(const std::string& path);
	~FighterPlane();
};

