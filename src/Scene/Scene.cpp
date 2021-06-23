#include <imgui/imgui.h>
#include <GL/glew.h>
#include "Renderer.h"
#include "Scene.h"

namespace Scene
{
	SceneMenu::SceneMenu(Scene*& currentScene) :
		m_currentScene(currentScene),
		m_clear_color(0.1f, 0.1f, 0.1f, 1.0f)
	{
		m_renderer = std::make_unique<Renderer>();
	}
	void SceneMenu::OnRendering()
	{
		m_renderer->Clear();
	}
	void SceneMenu::OnImGuiRendering()
	{
		for (auto kv : m_scenes)
		{
			if (ImGui::Button(kv.first.c_str()))
				m_currentScene = kv.second();
		}
		ImGui::ColorEdit3("Clear Color", (float*)&m_clear_color);
		GLCall(glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w));
		
	}
}