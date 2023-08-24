#include "ProjectileDummy.h"

Material* ProjectileDummy::ProjectileDummyMaterial;
Texture* ProjectileDummy::ProjectileDummyTexture;

ProjectileDummy::ProjectileDummy()
{
	ProjectileDummyObject = nullptr;
}

void ProjectileDummy::LoadProjectileDummyDefaults(const std::string& path) {
	if (ProjectileDummy::ProjectileDummyMaterial == nullptr)
		ProjectileDummy::ProjectileDummyMaterial = new Material();
	if (ProjectileDummy::ProjectileDummyTexture == nullptr) {
		ProjectileDummy::ProjectileDummyTexture = new Texture();
		ProjectileDummy::ProjectileDummyTexture->LoadTexture((path + "/Missile_texture.png").c_str());
		ProjectileDummy::ProjectileDummyMaterial->AttachTexture(ProjectileDummy::ProjectileDummyTexture);
	}
}

void ProjectileDummy::Load(const std::string& path, const std::string& model_name)
{
	std::string load_model_name = "Missile.fbx";
	if (model_name != "")
		load_model_name = model_name;
	ProjectileDummyObject = ResourceManager::LoadObject(path + "/" + load_model_name);
	ProjectileDummyObject->AssignMaterial(ProjectileDummy::ProjectileDummyMaterial);
}

ProjectileDummy::~ProjectileDummy()
{
	ResourceManager::DestroyObject(ProjectileDummyObject);
	delete ProjectileDummyObject;
}
