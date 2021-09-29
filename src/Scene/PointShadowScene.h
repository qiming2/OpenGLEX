#pragma once
#include "Common.h"
#include "Scene.h"
#include "CameraFps.h"
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
		CameraFps camera;
		unsigned int depthCubeMap;
		glm::mat4 model;
		
		
		
	};

}

