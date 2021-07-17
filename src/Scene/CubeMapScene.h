#pragma once
#include "Scene.h"
#include "CubeMap.h"
#include "CameraFps.h"
#include "Mesh.h"

namespace Scene {
	class CubeMapScene : public Scene
	{
	public:
		CubeMapScene();
		~CubeMapScene();

		void OnUpdate(float deltaTime);
		void OnRendering();
		void OnImGuiRendering();
	private:
		CameraFps camera;
		m_Shader normalShader;
		m_Shader skyboxShader;
		glm::mat4 model;
		Mesh cube;
		Mesh skybox;
		float scale;
		CubeMap cm;
	};
}

