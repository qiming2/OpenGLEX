#include <iostream>
#include <GL\glew.h>
#include "stb_image.h"
#include "Texture.h"

Texture::Texture(const char* image, GLenum activeID)
{
	// Create a texture
	m_activeID = activeID;
	glGenTextures(1, &m_RendererID);
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set linear to both mipmap and texture interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// load image and retrive necessary information
	int width, height, nChannels;
	// Usually we want to flip the picture
	// May not be true for some cases, be careful!!
	stbi_set_flip_vertically_on_load(true);
	// Always pad image to a multiple of 4 in width
	unsigned char* data = stbi_load(image, &width, &height, &nChannels, 4);
	// glPixelStorei(GL_UNPACK_ALIGHNMENT, #) maybe used if image width is not multiple of 4, this is very important!
	if (data)
	{
		// Configure Texture
		// pass loaded image data to bounded texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		// Generate Mipmap for avoiding artifacts when objects are small
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "TEXTURE::LOAD_FAILED\n" << "File: " << __FILE__ << std::endl;
	}
	// free image resource
	stbi_image_free(data);
	Unbind();

}

void Texture::SetActiveID(GLenum activeID)
{
	m_activeID = activeID;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind() const
{
	glActiveTexture(m_activeID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

