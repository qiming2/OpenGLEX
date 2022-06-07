#pragma once
#include "Scene.h"

namespace Scene {
	class PBRScene : public Scene
	{
	public:
		PBRScene();
		~PBRScene();
		void OnUpdate(float deltaTime) override;
		void OnRendering() override;
		void OnImGuiRendering() override;
	private:
		m_Shader pbrShader;
		Texture albedo_tex;
		Texture metallic_tex;
		Texture roughness_tex;
		//Texture normal;

	};


}

