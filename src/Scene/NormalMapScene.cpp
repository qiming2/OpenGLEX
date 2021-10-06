#include "NormalMapScene.h"
#include "Texture.h"
#include "Mesh.h"

namespace Scene {

	static std::string brick() {
		Texture::SetFlip(false);
		return std::string("res/Texture/brickwall.jpg");
	}

NormalMapScene::NormalMapScene():
	normalMap(brick().c_str(), 0),
	diffuseMap("res/Texture/brickwall_normal.jpg", 1),
	shader("res/shaders/normal_map_vert.shader", "res/shaders/normal_map_frag.shader")
{
	std::vector<float> plane = CreatePlane();
	std::vector<Vertex> vertices;
	vertices.reserve(plane.size() / 8);
	Vertex cur;
	for (int i = 0; i < plane.size() / 8; i+=8) {
		cur.position = glm::vec3(plane[i], plane[i + 1], plane[i + 2]);
		cur.normal = glm::vec3(plane[i+3], plane[i + 4], plane[i + 5]);
		cur.texCoord = glm::vec2(plane[i + 6], plane[i + 7]);
		// calculate per vertex tangent and bitangent using
		// normal and positions
		
	}
	shader.Bind();
	shader.SetInt("texture_diffuse1", 0);
	shader.SetInt("texture_normal1", 1);

	// Set up vertex array and indices array
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5 };
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, &indices[0], GL_STATIC_DRAW);

	// glGen
}

NormalMapScene::~NormalMapScene() {

}

void NormalMapScene::OnRendering() {

}

void NormalMapScene::OnImGuiRendering() {

}

void NormalMapScene::OnUpdate(float deltaTime) {

}


}