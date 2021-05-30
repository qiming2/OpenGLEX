#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TransformScene.h"
#include "Engine/Renderer.h"
#include "Engine/VertexBufferLayout.h"




namespace Scene {
	TransformScene::TransformScene()
	{
		std::vector<float> vertices =
		{
			// positions          // colors           // texture coords
		 0.71f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.71f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.71f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.71f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};

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
		// Color Attri
		layout.Push<float>(3);
		// Texture Attri
		layout.Push<float>(2);
		// Add vertexbuffer using layout object functions
		m_va->AddBuffer(*m_vb, layout);

		// Use relative path
		m_shader = std::make_unique<m_Shader>("res/shaders/b_vert_transform.shader", "res/shaders/b_frag_transform.shader");
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
	}

	TransformScene::~TransformScene()
	{
		// Unique pointer helps to manage memory deletion
	}

	void TransformScene::OnUpdate(float deltaTime)
	{
		
	}

	void TransformScene::OnRendering()
	{
		m_texture1->Bind();
		m_texture2->Bind();
		m_shader->Bind();
		m_shader->SetMat4fv("transform", glm::value_ptr(transform));
		// shader.SetUniform4f("u_Color", r, 0.6f, 0.8f, 1.0f);

		// Draw
		m_renderer->Draw(*m_va, *m_ib);
	}

	void TransformScene::OnImGuiRendering()
	{
		if (ImGui::TreeNode("Transformations")) {
			if (ImGui::Button("Scale")) {
				// First create a identity matrix
				glm::mat4 trans = glm::mat4(1.0f);

				// Do whatever transformations we want on the previous matrix
				// Order matters a lot!
				transform = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
				transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				
			}
			ImGui::TreePop();
		}
	}
}

