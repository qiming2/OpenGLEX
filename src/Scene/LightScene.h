#pragma once
#include <memory>
#include <imgui/imgui.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Scene.h"
#include "Engine/Renderer.h"
#include <GLFW/glfw3.h>
#include "CameraFps.h"

namespace Scene {
	class LightScene : public Scene{
	public:
		LightScene();
		~LightScene();
		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<VertexArray> m_va;
		std::unique_ptr<VertexBuffer> m_vb;
		std::unique_ptr<IndexBuffer> m_ib;
		std::unique_ptr<m_Shader> m_shader;
		std::unique_ptr<Texture> m_texture;
		std::vector<glm::vec3> positions;
		glm::mat4 model;
		glm::vec3 lightColor;
		CameraFps camera;
	};
}