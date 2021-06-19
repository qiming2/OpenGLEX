#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LightScene.h"
#include <GLFW/glfw3.h>
#include "Engine/Renderer.h"
#include "Engine/VertexBufferLayout.h"
#include "Geometryutil.h"
#include "Common.h"
#include "CameraFps.h"

// Phong Shader not bling-phong
namespace Scene {
LightScene::LightScene():
	model(glm::mat4(1.0f)),
	lightColor(glm::vec3(1.0f, 1.0f, 1.0f)),
	lightPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	modelRotate(glm::mat4(1.0f))
{
	m_renderer = std::make_unique<Renderer>();
	m_va = std::make_unique<VertexArray>();
	m_va->Bind();
	std::vector<float> vertices = CreateCubeWithNormal();
	m_vb = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
	m_vb->Bind();
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_va->AddBuffer(*m_vb, layout);
	m_shader = std::make_unique<m_Shader>("res/shaders/light_vert.shader", "res/shaders/light_frag.shader");
	m_shader->Bind();
	m_texture = std::make_unique<Texture>("res/Texture/se_1.jpg", GL_TEXTURE0);
	m_texture1 = std::make_unique<Texture>("res/Texture/awesomeface.png", GL_TEXTURE0);
	glEnable(GL_DEPTH_TEST);
}

LightScene::~LightScene() {

}

void LightScene::OnUpdate(float deltaTime) {
	camera.processInput();
	model = glm::mat4(1.0f);
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelRotate = glm::mat4(1.0f);
	
	modelRotate = glm::rotate(modelRotate, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
	modelRotate = glm::rotate(modelRotate, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelRotate = glm::rotate(modelRotate, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	

	modelRotate = glm::translate(modelRotate, glm::vec3(1.0f, 1.0f, 1.0f));

	modelRotate = glm::scale(modelRotate, glm::vec3(0.3f, 0.3f, 0.3f));
	
	lightPos = glm::vec3(modelRotate * glm::vec4(lightPos, 1.0f));
}

void LightScene::OnImGuiRendering() {
	ImGui::SliderFloat("r", &lightColor[0], 0.0f, 1.0f);
	ImGui::SliderFloat("g", &lightColor[1], 0.0f, 1.0f);
	ImGui::SliderFloat("b", &lightColor[2], 0.0f, 1.0f);
}

void LightScene::OnRendering() {
	m_renderer->Clear();
	m_va->Bind();
	m_shader->Bind();
	m_texture->Bind();
	camera.SetViewProjectMat(m_shader.get());
	m_shader->SetMat3fv("normalM", glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(model)))));
	m_shader->SetVec3fv("viewPos", glm::value_ptr(camera.pos));
	m_shader->SetMat4fv("model", glm::value_ptr(model));
	m_shader->SetVec3fv("lightColor", glm::value_ptr(lightColor));
	m_shader->SetVec3fv("lightPos", glm::value_ptr(lightPos));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	m_shader->SetMat4fv("model", glm::value_ptr(modelRotate));
	m_texture1->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	modelRotate = glm::mat4(1.0f);
	modelRotate = glm::rotate(modelRotate, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
	modelRotate = glm::rotate(modelRotate, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	modelRotate = glm::rotate(modelRotate, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelRotate = glm::translate(modelRotate, glm::vec3(-1.0f, -1.0f, -1.0f));

	modelRotate = glm::scale(modelRotate, glm::vec3(0.3f, 0.3f, 0.3f));
	m_shader->SetMat4fv("model", glm::value_ptr(modelRotate));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

}