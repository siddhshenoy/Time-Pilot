#pragma once

#include "glm/glm/glm.hpp"

typedef struct Vertex {
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
}
Vertex;


enum class RenderType {
	ELEMENTS,
	ARRAYS
};