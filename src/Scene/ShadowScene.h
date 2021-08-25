#pragma once
#include "Scene/Scene.h"
#include "Common.h"
#include "Renderer.h"
#include "Mesh.h"
#include "CameraFps.h"

namespace Scene {
	class ShadowScene : public Scene
	{
	public:
		ShadowScene();
		~ShadowScene();
		
		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		void config();
		Mesh cube;
		Mesh plane;
		Mesh screenQ;
		m_Shader shadowP;
		m_Shader normalS;
		glm::mat4 model;
		glm::mat4 planeM;
		glm::mat4 lightProjectView;
		CameraFps camera;
		// depth map and framebuffer for shadow pass
		unsigned int ddtex;
		unsigned int ddfb;

		
	};
}



