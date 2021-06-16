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

namespace Scene{
	static float xPos = 0.0f;
	static float yPos = 0.0f;
	static bool firstTime = true;
	static float fov = 45.0f;
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::mat4 ViewPractice(glm::vec3 pos, glm::vec3 target, glm::vec3 up);

	CameraScene::CameraScene():
		cameraPos(glm::vec3(0.0f, 0.0f, 0.0f)),
		cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
		cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
		yaw(-90.0f),
		pitch(0.0f),
		lastX(0.0f),
		lastY(0.0f),
		sensitivity(0.05f),
		cameraSpeed(0.005f)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback); 
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
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
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

	CameraScene::~CameraScene() {
	
	}

	void CameraScene::OnUpdate(float deltaTime) {
		model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
// 		Rotate around y axis
// 				const float radius = 20.0f;
// 				float camX = sin(glfwGetTime()) * radius;
// 				float camZ = cos(glfwGetTime()) * radius;
// 				view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		// Move using inputkeys
		if (!firstTime) {
			yaw += (xPos - lastX) * sensitivity;
			// ypos on computer ranges from bottom from top
			pitch += (lastY - yPos) * sensitivity;
		}
		firstTime = false;
		lastX = xPos;
		lastY = yPos;
		if (pitch > 89.0f) {
			pitch = 89.0f;
		} else if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		projection = glm::perspective(glm::radians(fov), 1080.0f / 1080.0f, 0.1f, 100.0f);
		cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront.y = sin(glm::radians(pitch));
		cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(cameraFront);
		if (window != nullptr) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				cameraPos += cameraFront *  cameraSpeed * deltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				cameraPos -= cameraFront * cameraSpeed * deltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				cameraPos += glm::normalize(glm::cross(cameraUp, cameraFront)) * cameraSpeed * deltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
			}
		}
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void CameraScene::OnRendering() {
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

	void CameraScene::OnImGuiRendering() {
		
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		xPos = xpos;
		yPos = ypos;
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		fov -= yoffset;
		if (fov > 45.0f) {
			fov = 45.0f;
		} else if (fov < 1.0f) {
			fov = 1.0f;
		}
	}

	// Practice
	static glm::mat4 ViewPractice(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
		glm::mat4 ret(1.0f);

		return ret;
	}
}
