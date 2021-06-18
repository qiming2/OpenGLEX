#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CameraScene.h"
#include <GLFW/glfw3.h>
#include "Engine/Renderer.h"
#include "Engine/VertexBufferLayout.h"
#include "Geometryutil.h"
#include "Common.h"
#include "CameraFps.h"

namespace Scene{
	

	CameraScene::CameraScene()
	{
		std::vector<float> vertices = CreateCube();

		std::vector<unsigned int> indices = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};


		m_va = std::make_unique<VertexArray>();
		// Creating buffer and getting an index
		m_vb = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
		// Creating index buffer and getting an index
		m_ib = std::make_unique<IndexBuffer>(indices.data(), indices.size());
		VertexBufferLayout layout;
		// Position Attri
		layout.Push<float>(3);
		// Texture Attri
		layout.Push<float>(2);
		// Add vertexbuffer using layout object functions
		m_va->AddBuffer(*m_vb, layout);

		// Use relative path
		m_shader = std::make_unique<m_Shader>("res/shaders/b_vert_coordinate.shader", "res/shaders/b_frag_coordinate.shader");
		m_shader->Bind();
		m_shader->SetInt("texture1", 0);

		m_shader->SetInt("texture2", 1);

		// shader.SetUniform4f("u_Color", 0.3f, 0.7f, 0.8f, 1.0f);
		// shader.SetUniform4f("a_Color", 0.1f, 0.1f, 0.1f, 1.0f);


		m_renderer = std::make_unique<Renderer>();


		m_texture1 = std::make_unique<Texture>("res/Texture/wall.jpg", GL_TEXTURE0);
		m_texture2 = std::make_unique<Texture>("res/Texture/awesomeface.png", GL_TEXTURE1);
		m_texture1->Bind();
		m_texture2->Bind();

// 		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); 
// 		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
// 		// Actually this is a reversed direction of what we have intended
// 		glm::vec3 cameraDirection = glm::vec3(cameraPos - cameraTarget);
// 		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
// 		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
// 		// Remember that cameraDirection is reversed
// 		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
// 		// This is a view matrix calcualted by up, cameraRight and front direction
// 		// Transposed rotation matrix * negated translation matrix

		// model
		//model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		positions = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		glEnable(GL_DEPTH_TEST);
	}

	CameraScene::~CameraScene() {
	
	}

	void CameraScene::OnUpdate(float deltaTime) {
		camera.processInput();
// 		Rotate around y axis
// 				const float radius = 20.0f;
// 				float camX = sin(glfwGetTime()) * radius;
// 				float camZ = cos(glfwGetTime()) * radius;
// 				view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		// Move using inputkeys
		
	}

	void CameraScene::OnRendering() {
		m_va->Bind();
		m_renderer->Clear();
		m_texture1->Bind();
		m_texture2->Bind();
		m_shader->Bind();

		camera.SetViewProjectMat(m_shader.get());
		// simple third person camera
// 		camera.pos = positions[0] + glm::vec3(glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(10.0f, 0.0f, 0.0f, 0.0f));
// 		camera.target = positions[0];
		//m_shader->SetMat4fv("view", glm::value_ptr(camera.getView()));
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			glm::vec3 cur_pos = positions[i];
			model = glm::translate(glm::mat4(1.0f), cur_pos);
			float angle = 20.0f * i;
			if (i < 3)
			{
				model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
				

			}
			else
			{
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			m_shader->SetMat4fv("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// shader.SetUniform4f("u_Color", r, 0.6f, 0.8f, 1.0f);
		// Draw
	}

	void CameraScene::OnImGuiRendering() {
		
	}

	
}
