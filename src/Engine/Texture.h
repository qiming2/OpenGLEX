#pragma once
#include <GL/glew.h>
#include <string>

enum gl_texture_flag {
	HDR = 0x01,
	NOT_FLIP_VERTICAL = 0x02,
};

class Texture
{
public:
	static bool flip;
	static void SetFlip(bool flip);
	Texture(const char* image, GLenum activeID);
	Texture(const char* image, GLenum activeID, const std::string& typeName);
	Texture(const char* image, GLenum activeID, unsigned int flag);
	~Texture();
	void SetActiveID(GLenum activeID);
	void Bind() const;
	void Unbind() const;
	void Delete() const;
	std::string path;
	std::string type;
	unsigned int m_RendererID;
private:
	GLenum m_activeID;
};