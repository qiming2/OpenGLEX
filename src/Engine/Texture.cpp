#include <iostream>
#include "stb_image.h"
#include "Texture.h"

Texture::Texture(const char* image, unsigned int activeID)
{
	// Create a texture
	m_activeID = GL_TEXTURE0 + activeID;
	glGenTextures(1, &m_RendererID);
	
	
	// load image and retrive necessary information
	int width, height, nChannels;
	// Usually we want to flip the picture
	// May not be true for some cases, be careful!!
	stbi_set_flip_vertically_on_load(Texture::flip);
	// Always pad image to a multiple of 4 in width
	unsigned char* data = stbi_load(image, &width, &height, &nChannels, 0);
	//maybe used if image width is not multiple of 4, this is very important!
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	if (data)
	{
		// Configure Texture
		// pass loaded image data to bounded texture object
		GLenum channel_type = GL_RGB;
		if (nChannels == 1) {
			channel_type = GL_RED;
			
		} else if (nChannels == 3) {
			
			channel_type = GL_RGB;
		} else if (nChannels == 4) {
			channel_type = GL_RGBA;
		}
		Bind();
		// first channel type should vary depending on what textures we are retrieving

		// for example if it is diffuse texture, usually artists would
		// tweak it in sRGB space to make it look good, when we are using
		// it for lighting calculation, we need to raise to the power of the gamma
		// before doing linear operation

		// for other usages like normal, bump maps, they are not created to represent
		// colors, thus don't need to convert them to linear space
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

static int load_hdr_image(const char* image, unsigned int flag, unsigned int rendererID) {
	int width, height, nrChannels;
	float* data = stbi_loadf(image, &width, &height, &nrChannels, 0);
	
	if (data) {
		glBindTexture(GL_TEXTURE_2D, rendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "FAILED TO LOAD HDR IMAGE: " << image << std::endl;
	}
	return 0;
}

Texture::Texture(const char* image, GLenum activeID, unsigned int flag) {
	m_activeID = GL_TEXTURE0 + activeID;
	glGenTextures(1, &m_RendererID);

	if (!(flag & gl_texture_flag::NOT_FLIP_VERTICAL))
		stbi_set_flip_vertically_on_load(true);

	if (flag & gl_texture_flag::HDR)
		load_hdr_image(image, flag, m_RendererID);
	
}

Texture::Texture(const char* image, GLenum activeID, const std::string& typeName)
	:Texture(image, activeID)
{
	//////////////////////////////// WARNING /////////////////////////
	// This is not Java!!!! Can not call this:
	// 	   Texture(image, activeID)
	// Have to use delegating constructor!!!!!
	///////////////////////////////// WARNING /////////////////////////
	type = typeName;
	
}

void Texture::SetActiveID(unsigned int activeID)
{
	m_activeID = GL_TEXTURE0 + activeID;
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