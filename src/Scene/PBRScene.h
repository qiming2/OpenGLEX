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
		Texture normal;
		
		m_Shader hdrToMap;
		Texture diffu_radiance_tex;

		m_Shader cube_shader;
		m_Shader irradiance_shader;

		m_Shader specular_shader;

		m_Shader brdf_shader;
		m_Shader quad_shader;
	};


}

