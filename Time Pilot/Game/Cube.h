#pragma once

#include <glm/glm/glm.hpp>

class Cube
{
private:
	glm::vec3 mCentre;
	glm::vec3 mDimensions;
public:
	Cube();
	Cube(const glm::vec3& centre, const glm::vec3& dimensions);
	void SetCentre(const glm::vec3& centre);
	void SetDimensions(const glm::vec3& dimensions);
	bool DoesPointFallInCube(const glm::vec3& point);
	~Cube();
};

