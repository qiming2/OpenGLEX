#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CoordinateScene.h"
#include <GLFW/glfw3.h>
#include "Engine/Renderer.h"
#include "Engine/VertexBufferLayout.h"
#include "FileUtil.h"
#include "Geometryutil.h"


namespace Scene
{
	CoordinateScene::CoordinateScene()
	{
		std::vector<float> vertices = std::move(CreateCube());
		std::vector<unsigned int> indices = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};


		m_va = std::make_unique<VertexArray>();
 		// Creating buffer and getting an index
		m_vb = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
 		// Creating index buffer and getting an index

		VertexBufferLayout layout;
 		// Position Attri
		layout.Push<float>(3);
 		// Texture Attri
		layout.Push<float>(2);
 		// Add vertexbuffer using layout object functions
		m_va->AddBuffer(*m_vb, layout);

		// Use r elative path
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

		// view matrix
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		// projection matrix
		projection = glm::perspective(glm::radians(45.0f), 1080.0f / 1080.0f, 0.1f, 100.0f);
		// model
		model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
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
	CoordinateScene::~CoordinateScene()
	{
		// Unique pointer helps to manage memory deletion
		m_va->Delete();
		m_vb->Delete();
		m_shader->Delete();
		m_texture1->Delete();
		m_texture2->Delete();
	}

	void CoordinateScene::OnUpdate(float deltaTime)
	{
		model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
	}

	void CoordinateScene::OnImGuiRendering()
	{
		// Nothing for now
	}

	void CoordinateScene::OnRendering()
	{
		m_va->Bind();
		m_renderer->Clear();
		m_texture1->Bind();
		m_texture2->Bind();
		m_shader->Bind();

		m_shader->SetMat4fv("view", glm::value_ptr(view));
		m_shader->SetMat4fv("projection", glm::value_ptr(projection));
		for (int i = 0; i < positions.size(); i++)
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
}