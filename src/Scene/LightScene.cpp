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
	lightPos(glm::vec3(1.2f, 1.0f, 2.0f)),
	modelRotate(glm::mat4(1.0f))
{
	Texture::SetFlip(true);
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
	m_shader2 = std::make_unique<m_Shader>("res/shaders/light_cube_vert.shader", "res/shaders/light_cube_frag.shader");
	m_shader = std::make_unique<m_Shader>("res/shaders/phong_vert.shader", "res/shaders/phong_frag.shader");
	m_shader->Bind();
	// Set static shader vals
	

	m_texture = std::make_unique<Texture>("res/Texture/se_1.jpg", GL_TEXTURE0);
	m_texture1 = std::make_unique<Texture>("res/Texture/awesomeface.png", GL_TEXTURE0);
	m_texture_container = std::make_unique<Texture>("res/Texture/container2.png", GL_TEXTURE0);
	// some funky specular maps
	m_texture_specular = std::make_unique<Texture>("res/Texture/container2_specular.png", GL_TEXTURE1);
	m_texture_emission = std::make_unique<Texture>("res/Texture/starsky.jpeg", GL_TEXTURE2);
	
	glEnable(GL_DEPTH_TEST);
}

LightScene::~LightScene() {
	m_va->Delete();
	m_vb->Delete();
	m_shader->Delete();
	m_shader2->Delete();
	m_texture->Delete();
	m_texture1->Delete();
	m_texture_container->Delete();
	m_texture_specular->Delete();
	m_texture_emission->Delete();
}

void LightScene::OnUpdate(float deltaTime) {
	camera.processInput();
	model = glm::mat4(1.0f);
	// Experimenting rotating cubes
	/*model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));*/
	modelRotate = glm::mat4(1.0f);
	
	modelRotate = glm::rotate(modelRotate, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
	modelRotate = glm::rotate(modelRotate, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelRotate = glm::rotate(modelRotate, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	

	modelRotate = glm::translate(modelRotate, lightColor);

	modelRotate = glm::scale(modelRotate, glm::vec3(0.3f, 0.3f, 0.3f));
	
	lightPos = glm::vec3(modelRotate * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void LightScene::OnImGuiRendering() {
	ImGui::SliderFloat("r", &lightColor[0], 0.0f, 1.0f);
	ImGui::SliderFloat("g", &lightColor[1], 0.0f, 1.0f);
	ImGui::SliderFloat("b", &lightColor[2], 0.0f, 1.0f);
}

void LightScene::OnRendering() {
	m_renderer->Clear();

	// Material Uniform
	//m_shader->setvec3fv("material.ambient", 1.0f, 0.5f, 0.31f);
	//m_shader->setvec3fv("material.diffuse", 1.0f, 0.5f, 0.31f);
	m_shader->Bind();
	m_shader->SetFloat("material.shininess", 16.0f);
	m_shader->SetInt("material.texture_container", 0);
	m_shader->SetInt("material.texture_specular", 1);
	m_shader->SetInt("material.texture_emission", 2);
	m_shader->SetFloat("u_time", (float)glfwGetTime());
	m_va->Bind();
	m_texture_container->Bind();
	m_texture_specular->Bind();
	m_texture_emission->Bind();
	// Camera
	camera.SetViewProjectMat(m_shader.get());
	m_shader->SetMat3fv("normalM", glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(model)))));
	m_shader->SetVec3fv("viewPos", glm::value_ptr(camera.pos));
	m_shader->SetMat4fv("model", glm::value_ptr(model));

	// Some experiment of light
	lightColor = glm::vec3(1.5f);
	// Change lightColor over time
	/*lightColor.x = sin(glfwGetTime() * 2.0f);
	lightColor.y = sin(glfwGetTime() * 0.7f);
	lightColor.z = sin(glfwGetTime() * 1.3f);

	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);*/
	// Keep light color at 1.0f for all light attributes
	glm::vec3 diffuseColor = lightColor;
	glm::vec3 ambientColor = lightColor;
	// Set Light Attributes
	m_shader->SetVec3fv("light.pos", glm::value_ptr(lightPos));
	m_shader->SetVec3fv("light.ambient", glm::value_ptr(0.3f * ambientColor));
	m_shader->SetVec3fv("light.diffuse", glm::value_ptr(0.5f * diffuseColor)); // darken diffuse light a bit
	m_shader->SetVec3fv("light.specular", 1.0f, 1.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// First one light cube
	m_shader2->Bind();
	camera.SetViewProjectMat(m_shader2.get());
	m_shader2->SetMat4fv("model", glm::value_ptr(modelRotate));
	m_shader2->SetVec3fv("lightColor", glm::value_ptr(lightColor));
	m_texture1->Bind();

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Second light cube
	//modelRotate = glm::mat4(1.0f);
	/*modelrotate = glm::rotate(modelrotate, (float)glfwgettime(), glm::vec3(1.0f, 0.0f, 0.0f));
	modelrotate = glm::rotate(modelrotate, (float)glfwgettime(), glm::vec3(0.0f, 1.0f, 0.0f));
	modelrotate = glm::rotate(modelrotate, (float)glfwgettime(), glm::vec3(0.0f, 0.0f, 1.0f));
	modelrotate = glm::translate(modelrotate, glm::vec3(-1.0f, -1.0f, -1.0f));*/

	//modelRotate = glm::scale(modelRotate, glm::vec3(0.3f, 0.3f, 0.3f));
	//m_shader2->SetMat4fv("model", glm::value_ptr(modelRotate));
	//glDrawArrays(GL_TRIANGLES, 0, 36);
}

}