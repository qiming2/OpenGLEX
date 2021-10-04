#pragma once
#include "Common.h"
#include "Scene.h"
#include "CameraFps.h"
#include "Mesh.h"
namespace Scene {
	
	class PointShadowScene : public Scene
	{
	public:
		PointShadowScene();
		~PointShadowScene();
		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		void renderScene(m_Shader& shader);
		CameraFps camera;
		unsigned int depthCubeMap;
		unsigned int depthfbo;

		glm::mat4 model;

		glm::mat4 shadowProj;

		m_Shader m_shadow_map_shader;
		m_Shader m_phong;

		Mesh cube;
		Mesh plane;

		//Texture diffuse;
	};

}

