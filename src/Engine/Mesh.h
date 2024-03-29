#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Engine/VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "m_Shader.h"
#include "Texture.h"


struct Vertex {
	// pos
	glm::vec3 position;
	// normal
	glm::vec3 normal;
	// texture coordinates
	glm::vec2 texCoord;
	// tangent
	glm::vec3 tangent;
	// bitangent
	glm::vec3 bitangent;
};


enum class MeshType:char {
	Quad,
	Cube,
	Sphere,
	Skybox
};

class Mesh
{
public:
	
	// For educational purposes, might not be optimized
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	Mesh(MeshType type);
	~Mesh();
	void ChangeTexture(const char* image, unsigned int index);
	void ChangeTexture(const Texture& image, unsigned int index);
	void Draw(m_Shader& shader);
	void DrawInstances(m_Shader& shader, int num);
	void RawDraw(m_Shader& shader);
	void Delete();
	// For customization purpose
	VertexArray& GetVa();
private:
	 /*Mesh data
	 use mesh class to structurize imported model
	 with assimp*/

	std::vector<Texture> textures;
	IndexBuffer ib;
	VertexArray va;
	VertexBuffer vb;
	void LoadRawVerticesWithNT(const std::vector<float>& vertices);
};