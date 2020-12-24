#pragma once
#include <memory>
#include <imgui/imgui.h>
#include "Scene.h"
#include "Engine/Renderer.h"

namespace Scene
{
	class TextureScene : public Scene
	{
	public:
		TextureScene();
		~TextureScene();

		void OnUpdate(float deltaTime) override;
		void OnRendering() override;
		void OnImGuiRendering() override;
	private:
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<VertexArray> m_va;
		std::unique_ptr<VertexBuffer> m_vb;
		std::unique_ptr<IndexBuffer> m_ib;
		std::unique_ptr<m_Shader> m_shader;
		std::unique_ptr<Texture> m_texture1;
		std::unique_ptr<Texture> m_texture2;
		float m_mix;
		ImVec4 m_clear_color;
		bool m_show_another_window;
		bool m_show_demo_window;
	};
}