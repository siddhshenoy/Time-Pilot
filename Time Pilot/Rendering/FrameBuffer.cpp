#include "FrameBuffer.h"


FrameBuffer::FrameBuffer() {
	mTransform = glm::mat4(1.0f);
}
void FrameBuffer::BindBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}
void FrameBuffer::CreateFrameBuffer(int width, int height) {
	mWidth = width;
	mHeight = height;
	spdlog::info("Creating framebuffer with size: " + std::to_string(width) + ", " + std::to_string(height));
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureID, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenRenderbuffers(1, &mRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO);
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		spdlog::error("OpenGL framebuffer creation failed, error: " + std::to_string(status));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	
	glBindVertexArray(mVAO);
	

	float vertices[] = {
		 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void FrameBuffer::UpdateFrameBuffer(int width, int height) {
	mWidth = width;
	mHeight = height;
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureID, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
Shader* FrameBuffer::GetShader() {
	return mShader;
}
void FrameBuffer::AttachShader(Shader* shader) {
	mShader = shader;
}

void FrameBuffer::SetTransform(const glm::mat4& transform)
{
	mTransform = transform;
}
glm::mat4 FrameBuffer::GetTransform()
{
	return mTransform;
}
void FrameBuffer::RenderFrameBuffer() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	mShader->UseShader();
	mShader->SetUniform1f("screenWidth", mWidth);
	mShader->SetUniform1f("screenHeight", mHeight);
	mShader->SetUniformMatrix("frame_transform", 1, GL_FALSE, &mTransform[0][0]);
	glBindVertexArray(mVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}
void FrameBuffer::UnbindBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
FrameBuffer::~FrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteTextures(1, &mTextureID);
	glDeleteRenderbuffers(1, &mRBO);
	glDeleteFramebuffers(1, &mFBO);
}