#pragma once

#include <glm/glm/glm.hpp>

#include <glew/GL/glew.h>

#include <spdlog/spdlog.h>

#include <Rendering/Shader.h>

class FrameBuffer
{
private:
	GLuint mFBO;
	GLuint mTextureID;
	GLuint mRBO;

	GLuint mVAO;
	GLuint mVBO;

	int mWidth;
	int mHeight;

	Shader* mShader;

	glm::mat4 mTransform;

public:
	FrameBuffer();
	void AttachShader(Shader* shader);
	Shader* GetShader();
	void CreateFrameBuffer(int width, int height);
	void UpdateFrameBuffer(int width, int height);
	void SetTransform(const glm::mat4& transform);
	glm::mat4 GetTransform();
	void BindBuffer();
	void UnbindBuffer();
	void RenderFrameBuffer();
	~FrameBuffer();
};

