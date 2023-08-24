#pragma once


#include <Game/Behaviour.h>

#include <Resource/ResourceManager.h>

class ProjectileDummy
{
public:
	Object* ProjectileDummyObject;
public:
	static Material* ProjectileDummyMaterial;
	static Texture* ProjectileDummyTexture;
public:
	static void LoadProjectileDummyDefaults(const std::string& path);
	
public:
	ProjectileDummy();
	void Load(const std::string& path, const std::string& model_name = "");
	~ProjectileDummy();
};

