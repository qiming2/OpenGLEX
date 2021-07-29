#pragma once
#include "Scene.h"
#include "Model.h"
#include "CameraFps.h"
#include "CubeMap.h"

namespace Scene {
	class InstanceIIScene : public Scene
	{
	public:
		InstanceIIScene();
		~InstanceIIScene();

		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		m_Shader shader;
		m_Shader instanceS;
		glm::mat4 model;
		VertexBuffer vb;
		CameraFps camera;
		Model rock;
		Model asteroid;
		CubeMap cm;
		m_Shader cm_shader;
		Mesh cube;
	};

}


