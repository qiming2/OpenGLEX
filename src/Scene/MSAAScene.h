#pragma once
#include "Scene.h"
#include "Mesh.h"
#include "Common.h"
#include "CameraFps.h"

namespace Scene {
	class MSAAScene : public Scene
	{
	public:
		MSAAScene();
		~MSAAScene();
		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		Mesh cube;
		m_Shader shader;
		glm::mat4 model;
		CameraFps camera;
		unsigned int msfb;
		unsigned int mstex;
		unsigned int rbo;


		// screen quad
		VertexArray screen_va;
		VertexBuffer screen_vb;
		IndexBuffer screen_ib;
		m_Shader screen_shaderMS;

		unsigned int currentfb;
	};

}


