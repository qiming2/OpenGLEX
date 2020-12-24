#include <imgui/imgui.h>
#include <GL/glew.h>
#include "Renderer.h"
#include "Scene.h"

namespace Scene
{
	SceneMenu::SceneMenu(Scene*& currentScene) :
		m_currentScene(currentScene),
		m_clear_color(0.3f, 0.7f, 0.2f, 1.0f)
	{

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