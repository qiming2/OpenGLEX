#pragma once
#include "Scene.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "CameraFps.h"

namespace Scene {
	class FramebufferScene : public Scene
	{
	public:
		FramebufferScene();
		~FramebufferScene();
		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		FrameBuffer fb;
		m_Shader normalShader;
		m_Shader screenShader;
		CameraFps camera;
		glm::mat4 model_m;
		Mesh cube;
		Mesh floor;
		int kernel_type;

		// Since screen quad is very different
		// from other render objects
		// we manually make it here
		VertexArray screen_va;
		VertexBuffer screen_vb;
		IndexBuffer screen_ib;
	};
}