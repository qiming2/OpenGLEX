#include <iostream>
#include "stb_image.h"
#include "Texture.h"

Texture::Texture(const char* image, GLenum activeID)
{
	// Create a texture
	m_activeID = activeID;
	glGenTextures(1, &m_RendererID);
	
	
	// load image and retrive necessary information
	int width, height, nChannels;
	// Usually we want to flip the picture
	// May not be true for some cases, be careful!!
	stbi_set_flip_vertically_on_load(Texture::flip);
	// Always pad image to a multiple of 4 in width
	unsigned char* data = stbi_load(image, &width, &height, &nChannels, 0);
	//maybe used if image width is not multiple of 4, this is very important!
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	if (data)
	{
		// Configure Texture
		// pass loaded image data to bounded texture object
		GLenum channel_type;
		if (nChannels == 1) {
			channel_type = GL_RED;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		} else if (nChannels == 3) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			channel_type = GL_RGB;
		} else if (nChannels == 4) {
			channel_type = GL_RGBA;
		}
		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, channel_type, width, height, 0, channel_type, GL_UNSIGNED_BYTE, data);
		// Generate Mipmap for avoiding artifacts when objects are small


		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		
	}
	else
	{
		std::cout << "TEXTURE::LOAD_FAILED\n" << "File: " << __FILE__ << std::endl;
	}
	// free image resource
	stbi_image_free(data);
	Unbind();

}

Texture::Texture(const char* image, GLenum activeID, const std::string& typeName)
	:Texture(image, activeID)
{
	//////////////////////////////// WARNING /////////////////////////
	// This is not Java!!!! Can not call this:
	// 	   Texture(image, activeID)
	// Have to delegating constructor!!!!!
	///////////////////////////////// WARNING /////////////////////////
	type = typeName;
	
}

void Texture::SetActiveID(GLenum activeID)
{
	m_activeID = activeID;
}

void Texture::Delete() const {
	glDeleteTextures(1, &m_RendererID);
	//std::cout << "Texture " << m_RendererID << " getting destroyed" << std::endl;
}

Texture::~Texture()
{
	
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

// Static members
bool Texture::flip = true;
void Texture::SetFlip(bool flip) {
	Texture::flip = flip;
}