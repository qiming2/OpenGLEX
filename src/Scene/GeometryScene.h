#pragma once
#include "CubeMap.h"
#include "CameraFps.h"
#include "Mesh.h"
#include "Scene.h"

namespace Scene {
	class GeometryScene : public Scene
	{
	public:
		GeometryScene();
		~GeometryScene();

		void OnUpdate(float deltaTime);
		void OnRendering();
		void OnImGuiRendering();
	private:
		CameraFps camera;
		m_Shader normalShader;
		glm::mat4 model;
		VertexArray va;
		VertexBuffer vb;
	};
}

