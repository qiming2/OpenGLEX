#pragma once
#include "Scene/Scene.h"
#include "CameraFps.h"
#include "Model.h"

namespace Scene {
	class BlendScene : public Scene
	{
	public:
		BlendScene();
		~BlendScene();
		void OnUpdate(float deltaTime) override;
		void OnRendering() override;
		void OnImGuiRendering() override;
	private:
		CameraFps camera;
		glm::mat4 model_m;
		m_Shader shader;
		Mesh cube;
		Mesh quad;
	};
}


