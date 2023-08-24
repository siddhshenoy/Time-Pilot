#pragma once

#include <iostream>
#include <stb/stb_image.h>
#include <glew/GL/glew.h>

class Texture
{
private:
	GLuint mTextureID;
public:
	Texture();
	Texture(const char* texture_path);
	void LoadTexture(const char* texture_path);
	void BindTexture();
	void UnbindTexture();
	~Texture();
};
