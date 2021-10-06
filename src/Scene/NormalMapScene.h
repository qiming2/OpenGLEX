#pragma once
#include "Scene/Scene.h"
#include "Common.h"
#include "CameraFps.h"
namespace Scene {
	class NormalMapScene : public Scene
	{
	public:
		NormalMapScene();
		~NormalMapScene();
		void OnUpdate(float deltaTime) override;
		void OnRendering() override;
		void OnImGuiRendering() override;
	private:
		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;
		Texture normalMap;
		Texture diffuseMap;
		CameraFps camera;
		m_Shader shader;
	};
}


