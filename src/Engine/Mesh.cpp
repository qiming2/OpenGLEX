#include "Mesh.h"
#include <string>
#include <iostream>
#include "Common.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures):
	ib(indices.data(), indices.size()),
	vb(vertices.data(), vertices.size() * sizeof(Vertex))
{
	this->textures = textures;
	va.Bind();
	ib.Bind();
	vb.Bind();
	VertexBufferLayout layout;
	// position
	layout.Push<float>(3);
	// normal
	layout.Push<float>(3);
	// texture coordinates
	layout.Push<float>(2);
	// tangent
	layout.Push<float>(3);
	// bitangent
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);
	va.Unbind();
}

Mesh::~Mesh() {

}

void Mesh::Delete() {
	va.Delete();
	ib.Delete();
	vb.Delete();
	for (int i = 0; i < textures.size(); i++) {
		textures[i].Delete();
	}
	textures.clear();
}

void Mesh::Draw(m_Shader& shader) {
	// set a convertion for binding textures to
	// appropriate texture slot
	// with this naming convertion
	// we can define as many sampler in shader
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (int i = 0; i < textures.size(); i++) {
		// change active texture slot number for this texture
		// for a single mesh at any time, we rebind all the textures
		// to appropriate slots
		textures[i].SetActiveID(GL_TEXTURE0 + i);
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		} else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		} else if (name == "texture_normal") {
			number = std::to_string(normalNr++);
		} else if (name == "texture_height") {
			number = std::to_string(heightNr++);
		} else {
			std::cout << "ERROR:: TEXTURE::BINDING FAILED\n " << name << "DOES NOT MATCH ANY TEXTURE TYPE. File: " << __FILE__ << std::endl;
		}
		// Set current texture to correct sampler slot
		shader.SetInt(name + number, i);
		textures[i].Bind();
	}
	gl_renderer.Draw(va, ib);
    
	// Reset everything
	glActiveTexture(GL_TEXTURE0);
}