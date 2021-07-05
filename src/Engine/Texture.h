#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture(const char* image, GLenum activeID);

	~Texture();
	void SetActiveID(GLenum activeID);
	void Bind() const;
	void Unbind() const;
	void Delete() const;
	std::string path;
	std::string type;
private:
	unsigned int m_RendererID;
	GLenum m_activeID;
};