#pragma once
#include "Scene/Scene.h"
#include "CameraFps.h"
#include "Common.h"
#include "Mesh.h"
#include "Model.h"
namespace Scene {
	class DeferredShadingScene : public Scene
	{
	public:
		DeferredShadingScene();
		~DeferredShadingScene();
		void OnUpdate(float deltaTime) override;
		void OnRendering() override;
		void OnImGuiRendering() override;
	private:
		CameraFps camera;
		
		m_Shader gShader;
		m_Shader screenGShader;
		// model mat
		glm::mat4 model;

		m_Shader normalShader;
		m_Shader ssaoShader;	
		// Mesh
		Mesh cube;
		Model backpack;


		
		std::vector<glm::vec3> ssaoKernel;
		std::vector<glm::vec3> ssaoNoise;
		
	};
}

