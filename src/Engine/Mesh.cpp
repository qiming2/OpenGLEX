#include "Mesh.h"
#include <string>
#include <iostream>
#include "GeometryUtil.h"
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

Mesh::Mesh(MeshType type) {
	if (type == MeshType::Quad) {
		LoadRawVerticesWithNT(CreatePlane());
	} else if (type == MeshType::Cube) {
		LoadRawVerticesWithNT(CreateCubeWithNormal());
	}
}

// Load vertices created from GeomtryUtil
void Mesh::LoadRawVerticesWithNT(const std::vector<float>& vertices) {
	va.Bind();
	Vertex temp;
	glm::vec3 v_temp;
	glm::vec2 v_temp1;
	std::vector<unsigned int> indices;
	std::vector<Vertex> vertexData;
	unsigned int index = 0;
	unsigned int i = 0;
	while (i < vertices.size()) {
		///////////////////////////////////////// WARNING ///////////////////////////////////
		// Don't use this!!!! order of which i++ takes place first is not defined!
		//temp.position = glm::vec3(vertices[i++], vertices[i++], vertices[i++]);
		///////////////////////////////////////// WARNING ///////////////////////////////////

		// Pos
		v_temp.x = vertices[i++];
		v_temp.y = vertices[i++];
		v_temp.z = vertices[i++];
		temp.position = v_temp;

		// Normal
		v_temp.x = vertices[i++];
		v_temp.y = vertices[i++];
		v_temp.z = vertices[i++];
		temp.normal = v_temp;

		// Texture
		v_temp1.x = vertices[i++];
		v_temp1.y = vertices[i++];
		temp.texCoord = v_temp1;
		// Indices
		indices.push_back(index++);
		// Vertices remap
		vertexData.push_back(temp);
	}
	ib.ReMap(indices.data(), indices.size());
	vb.ReMap(vertexData.data(), vertexData.size() * sizeof(Vertex));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);
	// Bind one default texture for now
	textures.emplace_back("res/Texture/starsky.jpeg", GL_TEXTURE0, "texture_diffuse");
	va.Unbind();
}

Mesh::~Mesh() {

}

VertexArray& Mesh::GetVa() {
	return va;
}

void Mesh::ChangeTexture(const char* image, unsigned int index) {
	// if index is greater than the texture size, we don't currently
	// want this to happen
	if (index > textures.size() - 1) {
		std::cout << "ERROR::MESH::TEXTURE::CHANGE FAILED\n INDEX GREATER THAN CONTAINER SIZE " << __FILE__ << std::endl;
		return;
	}
	textures[index].Delete();
	textures[index] = Texture(image, GL_TEXTURE0 + index, "texture_diffuse");
}

void Mesh::ChangeTexture(const Texture& image, unsigned int index) {
	// if index is greater than the texture size, we don't currently
	// want this to happen
	if (index > textures.size() - 1) {
		std::cout << "ERROR::MESH::TEXTURE::CHANGE FAILED\n INDEX GREATER THAN CONTAINER SIZE " << __FILE__ << std::endl;
		return;
	}
	textures[index].Delete();
	textures[index] = image;
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

void Mesh::RawDraw(m_Shader& shader) {
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);
	va.Unbind();
	ib.Unbind();
}

void Mesh::Draw(m_Shader& shader) {
	// set a convertion for binding textures to
	// appropriate texture slot
	// with this naming convertion
	// we can define as many sampler in shader
	shader.Bind();
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
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);
	va.Unbind();
	ib.Unbind();
    
	// Reset everything
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::DrawInstances(m_Shader& shader, int num) {
	// set a convertion for binding textures to
	// appropriate texture slot
	// with this naming convertion
	// we can define as many sampler in shader
	shader.Bind();
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
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_normal") {
			number = std::to_string(normalNr++);
		}
		else if (name == "texture_height") {
			number = std::to_string(heightNr++);
		}
		else {
			std::cout << "ERROR:: TEXTURE::BINDING FAILED\n " << name << "DOES NOT MATCH ANY TEXTURE TYPE. File: " << __FILE__ << std::endl;
		}
		// Set current texture to correct sampler slot
		shader.SetInt(name + number, i);
		textures[i].Bind();
	}
	va.Bind();
	ib.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0, num);
	va.Unbind();
	ib.Unbind();
	// Reset everything
	glActiveTexture(GL_TEXTURE0);
}