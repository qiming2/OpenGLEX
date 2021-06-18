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

namespace Scene {
LightScene::LightScene() {
	std::vector<float> vertices = CreateCube();
	m_vb = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_va->AddBuffer(*m_vb, layout);
	m_shader == std::make_unique<m_Shader>("res/shaders/light_vert.shader", "res/shaders/light_frag.shader");

}

LightScene::~LightScene() {

}

void LightScene::OnUpdate(float deltaTime) {
	
}

void LightScene::OnImGuiRendering() {

}

void LightScene::OnRendering() {


}

}