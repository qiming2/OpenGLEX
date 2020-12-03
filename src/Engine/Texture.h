#pragma once
#include <GL/glew.h>

class Texture
{
public:
	Texture(const char* image, GLenum activeID);

	~Texture();
	void SetActiveID(GLenum activeID);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
	GLenum m_activeID;
};