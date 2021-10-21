#pragma once
#include "Scene.h"
#include "Common.h"
#include "Mesh.h"
#include "CameraFps.h"

namespace Scene {
	class HdrBloomScene : public Scene
	{
	public:
		HdrBloomScene();
		~HdrBloomScene();
		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		Mesh cube;
		Mesh quad;
		unsigned int screen_vao;
		unsigned int screen_vbo;
		glm::mat4 model;
		unsigned int colorBuffer[2];
		unsigned int hdrfbo;

		unsigned int blurfbo[2];
		unsigned int blurBuf[2];
		unsigned int depthrbo;
		m_Shader shader;
		m_Shader light_shader;
		m_Shader light_cube;
		m_Shader blur_pass;
		CameraFps camera;
	};

}

