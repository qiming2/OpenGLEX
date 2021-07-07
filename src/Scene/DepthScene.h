#pragma once
#include "Scene.h"
#include "CameraFps.h"
#include "Model.h"

namespace Scene {
	class DepthScene  : public Scene
	{
	public:
		DepthScene();
		~DepthScene();
		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		CameraFps camera;
		m_Shader shader;
		Model model;
		glm::mat4 model_m;
		VertexArray* va;
		VertexBuffer* vb;
		IndexBuffer* ib;
	};

}

