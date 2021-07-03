#pragma once
#include "Scene.h"
#include "Texture.h"
#include "CameraFps.h"
#include "Renderer.h"

// Gonna practice write all the rendering without using
// Engine classes

namespace Scene {
	class LightTypeScene : public Scene {
	public:
		LightTypeScene();
		~LightTypeScene();
		void OnUpdate(float deltaTime) override;
		void OnRendering() override;
		void OnImGuiRendering() override;
	private:
		unsigned int va_id;
		unsigned int vb_id;
		Texture* diffuseTexture;
		Texture* specularTexture;
		m_Shader* phong;
		m_Shader* light_cube;
		CameraFps camera;
		Renderer* renderer;
		glm::mat4 model;
		glm::mat4 model_light;
		glm::vec3 lightColor;
		glm::vec3 cubePositions[10];
	};
}