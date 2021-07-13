#pragma once
#include "GL/glew.h"
// Framebuffer class
// More often used for off-screen rendering
// and post processing
// and more!
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();
	void Bind();
	void BindTexture(GLenum activeID);
	void Unbind();
	void Delete();
	void Resize(int width, int height);
private:
	unsigned int m_RendererID;
	unsigned int rbo;
	unsigned int texColorBuffer;
};

