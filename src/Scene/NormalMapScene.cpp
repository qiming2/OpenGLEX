#include "NormalMapScene.h"
#include "Texture.h"
#include "Mesh.h"

namespace Scene {
	static bool parallexMapping = true;
	static float heightScale = 0.1f;
	static bool hardCode = false;
	static bool normalized = false;
	static glm::vec3 bi;
	static glm::vec3 ta;

NormalMapScene::NormalMapScene():
	diffuseMap("res/Texture/wood.png", 0),
	normalMap("res/Texture/toy_box_normal.png", 1),
	heightMap("res/Texture/toy_box_disp.png", 2),
	shader("res/shaders/normal_map_vert.shader", "res/shaders/normal_map_frag.shader")
{
	std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5 };
	std::vector<float> plane = CreatePlane();
	std::vector<Vertex> vertices;
	vertices.reserve(plane.size() / 8);
	{
		Vertex cur;

		// we first gather all vertices into Vertex struct
		for (int i = 0; i < plane.size(); i += 8) {
			cur.position = glm::vec3(plane[i], plane[i + 1], plane[i + 2]);
			cur.normal = glm::vec3(plane[i + 3], plane[i + 4], plane[i + 5]);
			cur.texCoord = glm::vec2(plane[i + 6], plane[i + 7]);

			vertices.push_back(cur);
		}

	}
	
	{
		// calculate tangent and bitangent based in indices buffer
		float determinant, deltaU1, deltaU2, deltaV1, deltaV2;
		glm::vec3 e1, e2, T, B;
		for (int i = 0; i < indices.size(); i+=3) {
			// calculate per vertex tangent and bitangent using
			// normal and positions

			// --------------------------- IMPORTANT ----------------------
			// If I need want to change something in the array, use reference!!!!!!!!!!!!!! Not change the copied ones!
			Vertex& v1 = vertices[indices[i]];
			Vertex& v2 = vertices[indices[i+1]];
			Vertex& v3 = vertices[indices[i+2]];
			deltaU1 = v2.texCoord.x - v1.texCoord.x;
			deltaV1 = v2.texCoord.y - v1.texCoord.y;
			deltaU2 = v3.texCoord.x - v1.texCoord.x;
			deltaV2 = v3.texCoord.y - v1.texCoord.y;
			e1 = v2.position - v1.position;
			e2 = v3.position - v1.position;
			determinant = 1.0 / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
			T.x = determinant * (deltaV2 * e1.x + -deltaV1 * e2.x);
			T.y = determinant * (deltaV2 * e1.y + -deltaV1 * e2.y);
			T.z = determinant * (deltaV2 * e1.z + -deltaV1 * e2.z);
			B.x = determinant * (-deltaU2 * e1.x + deltaU1 * e2.x);
			B.y = determinant * (-deltaU2 * e1.y + deltaU1 * e2.y);
			B.z = determinant * (-deltaU2 * e1.z + deltaU1 * e2.z);
			v1.bitangent = B;
			v1.tangent = T;
			v2.bitangent = B;
			v2.tangent = T;
			v3.bitangent = B;
			v3.tangent = T;
			/*std::cout << "NOOOOO: T1 " << i << ": " << v1.tangent << " NOOOOO: B1: " << v1.bitangent << std::endl;*/
		}

	}
	
	shader.Bind();
	shader.SetInt("texture_diffuse1", 0);
	shader.SetInt("texture_normal1", 1);
	shader.SetInt("texture_height1", 2);
	//// Set up vertex array and indices array
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	std::cout << vertices.size() * sizeof(Vertex) << std::endl;
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	//
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices.data(), GL_STATIC_DRAW);
	
	// glVertexAttrib
	// Position, Normal, texCoord, Tangent, Bitangent
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (const void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (const void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (const void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertex), (const void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex), (const void*)(11 * sizeof(float)));
	//std::cout << sizeof(Vertex) << std::endl;
	glBindVertexArray(0);
	glDeleteBuffers(1, &vbo);
}

NormalMapScene::~NormalMapScene() {
	//glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	shader.Delete();
	normalMap.Delete();
	diffuseMap.Delete();

}

void NormalMapScene::OnRendering() {
	gl_renderer.Clear();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	shader.Bind();
	shader.SetInt("tuto", 0);
	shader.SetInt("hardCode", hardCode);
	shader.SetInt("normalized", normalized);
	camera.SetViewProjectMat(shader);
	glActiveTexture(GL_TEXTURE0 + 0);
	diffuseMap.Bind();
	glActiveTexture(GL_TEXTURE0 + 1);
	normalMap.Bind();
	glActiveTexture(GL_TEXTURE0 + 2);
	heightMap.Bind();
	shader.SetInt("setNorm", 0);
	shader.SetInt("parallexMapping", parallexMapping);
	shader.SetMat4fv("model", model);
	shader.SetVec3fv("viewPos", camera.pos);
	shader.SetFloat("heightScale", heightScale);
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	shader.SetMat4fv("model", model);
	shader.SetInt("parallexMapping", 0);
	shader.SetInt("setNorm", 1);
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void NormalMapScene::OnImGuiRendering() {
	camera.OnImGuiRendering();
	ImGui::Checkbox("Parallex Mapping", &parallexMapping);
	ImGui::Checkbox("Hard Code", &hardCode);
	ImGui::Checkbox("Normalize?", &normalized);
	ImGui::SliderFloat("Height Scale factor", &heightScale, 0.05f, 1.0f);
}

void NormalMapScene::OnUpdate(float deltaTime) {
	camera.processInput();
}


}