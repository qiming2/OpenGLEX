#pragma once
#include "Scene/Scene.h"
#include "CameraFps.h"
#include "Common.h"
#include "Mesh.h"
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
		unsigned int Gfbo; // gbuffer fbo
		unsigned int pTex; // position Texture
		unsigned int nTex; // normal Texture
		unsigned int aTex; // albedo Texture
		unsigned int depthB;
		// unsigned int sTex; specular intensity, don't need this
		// since we can just store it in albedo texture
		unsigned int screenQuadVao;
		unsigned int vbo;
		unsigned int ibo;
		m_Shader gShader;
		m_Shader screenGShader;
		// model mat
		glm::mat4 model;

		m_Shader normalShader;
		// Mesh
		Mesh cube;
	};
}

