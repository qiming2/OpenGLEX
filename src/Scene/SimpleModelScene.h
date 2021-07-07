#pragma once
#include "Scene/Scene.h"
#include "CameraFps.h"
#include "Model.h"

namespace Scene {
	class SimpleModelScene : public Scene
	{
	public:
		SimpleModelScene();
		~SimpleModelScene();
		void OnUpdate(float deltaTime) override;
		void OnRendering() override;
		void OnImGuiRendering() override;
	private:
		CameraFps camera;
		Model model;
		Model barbara;
		glm::mat4 model_m;
		m_Shader shader;
	};
}


