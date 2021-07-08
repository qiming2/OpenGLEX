#pragma once
#include "Scene.h"
#include "CameraFps.h"
#include "Model.h"

namespace Scene {
	class StencilScene  : public Scene
	{
	public:
		StencilScene();
		~StencilScene();
		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		CameraFps camera;
		m_Shader shader;
		m_Shader outlineShader;
		Model model;
		glm::mat4 model_2;
		glm::mat4 model_m;
		void DrawObjects(m_Shader& shader);
		void DrawOutline(m_Shader& shader);
		void DrawSingleObject(glm::mat4& model_temp, m_Shader& shader);
		void DrawSingleOutline(glm::mat4& model_temp, m_Shader& shader);
		void OutlineStartSetting();
		void OutlineDrawSetting();
		void OutlineEndSetting();
	};

}

