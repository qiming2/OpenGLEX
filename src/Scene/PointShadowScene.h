#pragma once
#include "Common.h"
#include "Scene.h"
#include "CameraFps.h"
#include "Mesh.h"
namespace Scene {
	
	class PointShadowScene : public Scene
	{
		struct PointLight {
			glm::vec3 pos;
			glm::vec3 specular;
			glm::vec3 diffuse;
			glm::vec3 ambient;
		};
	public:
		PointShadowScene();
		~PointShadowScene();
		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		CameraFps camera;
		unsigned int depthCubeMap;
		unsigned int depthfbo;
		glm::mat4 model;
		glm::mat4 model2;
		glm::mat4 planeModel;
		glm::mat4 shadowProj;
		float near;
		float far;

		Mesh cube;
		Mesh cube2;
		Mesh plane;
		PointLight light;
	};

}

