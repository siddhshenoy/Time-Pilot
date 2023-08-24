#pragma once

#include <glm/glm/glm.hpp>

class CommonMath
{
public:
	static float DistanceBetweenPoints3D(const glm::vec3& point1, const glm::vec3& point2); 
	static float DistanceBetweenPoints2D(const glm::vec2& point1, const glm::vec2& point2);
	static float Length3D(const glm::vec3& vec);
	static float Length2D(const glm::vec2& vec);
	static float fast_inverse_square_root(float number);
	static float fast_square_root(float number);
};

