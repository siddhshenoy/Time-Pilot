#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

Texture::Texture()
{
	mTextureID = 0;
}

Texture::Texture(const char* texture_path)
{
	LoadTexture(texture_path);
}

void Texture::LoadTexture(const char* texture_path)
{

	glGenTextures(1, &mTextureID);

	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLint width, height, channel;
	unsigned char* data = stbi_load(texture_path, &width, &height, &channel, 4);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	} else {
		std::cerr << "Could not load the texture from path '" << texture_path << "'" << std::endl;
	}
	stbi_image_free(data);
}

void Texture::BindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTextureID);
}
