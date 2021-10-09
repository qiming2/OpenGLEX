#include "NormalMapScene.h"
#include "Texture.h"
#include "Mesh.h"

namespace Scene {
	static bool parallexMapping = 0;
	static float heightScale = 0.1f;
	static bool hardCode = false;
	static bool normalized = false;
	static glm::vec3 bi;
	static glm::vec3 ta;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void static renderQuad()
	{
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

NormalMapScene::NormalMapScene():
	diffuseMap("res/Texture/bricks2.jpg", 0),
	normalMap("res/Texture/bricks2_normal.jpg", 1),
	heightMap("res/Texture/bricks2_disp.jpg", 2),
	shader("res/shaders/normal_map_vert.shader", "res/shaders/normal_map_frag.shader")
{
	std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5 };
	std::vector<float> plane = CreatePlane();
	std::vector<Vertex> vertices;
	vertices.reserve(plane.size() / 8);
	{

		// ----------------------------------Tutorial-----------------------------
		if (quadVAO == 0)
		{
			// positions
			glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
			glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
			glm::vec3 pos3(1.0f, -1.0f, 0.0f);
			glm::vec3 pos4(1.0f, 1.0f, 0.0f);
			// texture coordinates
			glm::vec2 uv1(0.0f, 1.0f);
			glm::vec2 uv2(0.0f, 0.0f);
			glm::vec2 uv3(1.0f, 0.0f);
			glm::vec2 uv4(1.0f, 1.0f);
			// normal vector
			glm::vec3 nm(0.0f, 0.0f, 1.0f);

			// calculate tangent/bitangent vectors of both triangles
			glm::vec3 tangent1, bitangent1;
			glm::vec3 tangent2, bitangent2;
			// triangle 1
			// ----------
			glm::vec3 edge1 = pos2 - pos1;
			glm::vec3 edge2 = pos3 - pos1;
			glm::vec2 deltaUV1 = uv2 - uv1;
			glm::vec2 deltaUV2 = uv3 - uv1;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			
			// triangle 2
			// ----------
			edge1 = pos3 - pos1;
			edge2 = pos4 - pos1;
			deltaUV1 = uv3 - uv1;
			deltaUV2 = uv4 - uv1;

			f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


			bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


			/*glm::vec3 _tangent1 = glm::mat3(model) * tangent1;
			glm::vec3 _bitangent1 = glm::mat3(model) * bitangent1;
			glm::vec3 _tangent2 = glm::mat3(model) * tangent1;
			glm::vec3 _bitangent2 = glm::mat3(model) * bitangent1;*/
			/*std::cout << "Tutorial: T1: " << _tangent1 << "Tutorial: T2: " << _tangent2 << "Tutorial: B1: " << _bitangent1 << "Tutorial: B2: " << _bitangent2 << std::endl;*/
			float quadVertices[] = {
				// positions            // normal         // texcoords  // tangent                          // bitangent
				pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
				pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
				pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

				pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
				pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
				pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
			};

			std::cout << sizeof(quadVertices) << std::endl;
			// configure plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
		}

		// ------------------------------------Tutorial above-----------
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
	std::cout << sizeof(Vertex) << std::endl;

	glBindVertexArray(0);
}

NormalMapScene::~NormalMapScene() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	shader.Delete();
	normalMap.Delete();
	diffuseMap.Delete();

}

void NormalMapScene::OnRendering() {
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	gl_renderer.Clear();
	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	shader.Bind();
	camera.SetViewProjectMat(shader);
	shader.SetInt("tuto", 1);
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
	//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	renderQuad();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.SetMat4fv("model", model);
	shader.SetInt("parallexMapping", 0);
	shader.SetInt("setNorm", 1);
	//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	renderQuad();
	glBindVertexArray(0);
	// ----------------------------------- Tutorial vs Mine ---------------------------
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
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