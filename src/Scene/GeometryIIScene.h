#pragma once
#include "Scene.h"
#include "CameraFps.h"
#include "Model.h"
#include "UniformBuffer.h"

namespace Scene {
	class GeometryIIScene : public Scene
	{
	public:
		GeometryIIScene();
		~GeometryIIScene();
		void OnUpdate(float deltaTime) override;
		void OnRendering() override;
		void OnImGuiRendering() override;
	private:
		Model ayaka;
		Mesh cube;
		m_Shader shader;
		m_Shader normal_shader;
		glm::mat4 model;
		CameraFps camera;
		// try use uniform buffer for projection and view
		UniformBuffer ub;
	};
}


