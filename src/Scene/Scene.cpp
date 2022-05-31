#include <imgui/imgui.h>
#include <utility>
#include <GL/glew.h>
#include "Renderer.h"
#include "Scene/SimpleModelScene.h"
#include "Scene.h"

#define PRIORITY_VAL 1000
#define MAX_CACHE_NUM 4

namespace Scene
{
	SceneMenu::SceneMenu(Scene*& currentScene) :
		m_currentScene(currentScene),
		m_clear_color(0.0f, 0.0f, 0.0f, 1.0f)
	{
		m_renderer = std::make_unique<Renderer>();
	}
	void SceneMenu::OnRendering()
	{
		m_renderer->Clear();
	}

	void SceneMenu::CleanUp() {
		std::string lowest = lowestPriority();
		if (lowest != "" && cacheNames.size() > MAX_CACHE_NUM) {
			delete cache.at(lowest);
			cache.erase(lowest);
			cacheNames.erase(lowest);
		}
	}
	
	void SceneMenu::ChangePriority(const std::string& name) {
		std::map<std::string, int>::iterator it;
		for (it = cacheNames.begin(); it != cacheNames.end(); it++) {
			if (it->first == name) {
				it->second = PRIORITY_VAL;
			} else {
				it->second--;
			}
		}
	}

	std::string SceneMenu::lowestPriority() {
		std::map<std::string, int>::iterator it;
		std::string lowest = "";
		int lowestPri = INT_MIN;
		for (it = cacheNames.begin(); it != cacheNames.end(); it++) {
			if (lowestPri == INT_MIN || it->second < lowestPri) {
				lowest = it->first;
				lowestPri = it->second;
			}
		}
		
		return lowest;
	}

	void SceneMenu::OnImGuiRendering()
	{
		for (auto kv : m_scenes)
		{
			
			if (ImGui::Button(kv.first.c_str())) {
				auto it = cache.find(kv.first);
				if (it != cache.end()) {
					m_currentScene = it->second;
				} else {
					Scene* newScene = kv.second();
					cache.emplace(kv.first, newScene);
					cacheNames.emplace(kv.first, PRIORITY_VAL);
					m_currentScene = newScene;
				}
				ChangePriority(kv.first);
			}
		}
		ImGui::ColorEdit3("Clear Color", (float*)&m_clear_color);
		GLCall(glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w));
		
	}
}