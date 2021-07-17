#include <iostream>
#include "CubeMap.h"
#include "stb/stb_image.h"

// will always load the 6 textures in such order:

//Texture target	Orientation
//GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right   0
//GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left	1
//GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top		2
//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom	3
//GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back	4
//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front 	5
CubeMap::CubeMap(const std::vector<std::string>& paths, GLenum activeID) {
	glGenTextures(1, &m_RendererID);
	m_activeID = activeID;
	stbi_set_flip_vertically_on_load(Texture::flip);
	
	int width, height, nChannels;
	unsigned char* data = nullptr;
	for (int i = 0; i < paths.size(); i++) {
		data = stbi_load(paths[i].c_str(), &width, &height, &nChannels, 0);
		if (data) {
			GLenum channel_type;
			if (nChannels == 1) {
				channel_type = GL_RED;
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			} else if (nChannels == 3) {
				channel_type = GL_RGB;
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			} else if (nChannels == 4) {
				channel_type = GL_RGBA;
			}
			Bind();
			// specify which face we targeting to,
			// and rest of tex info
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, channel_type, width, height, 0, channel_type, GL_UNSIGNED_BYTE, data);
		} else {
			std::cout << "CUBE TEXTURE::LOAD_FAILED\n" << "File: " << __FILE__ << std::endl;
		}
	}
	if (data) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		stbi_image_free(data);
	}
	Unbind();
}

CubeMap::~CubeMap() {

}

void CubeMap::SetActiveID(GLenum activeID)
{
	m_activeID = activeID;
}
void CubeMap::Delete() {
	glDeleteTextures(1, &m_RendererID);
}

void CubeMap::Bind() {
	glActiveTexture(m_activeID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
}

void CubeMap::Unbind() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}