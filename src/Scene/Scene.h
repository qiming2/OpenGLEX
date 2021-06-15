#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>
#include <imgui/imgui.h>
#include "Renderer.h"

namespace Scene
{
	class Scene
	{
	public:
		Scene() {};
		virtual ~Scene() {};

		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRendering() {};
		virtual void OnImGuiRendering() {};
	};

	class SceneMenu : public Scene
	{
	public:
		SceneMenu(Scene*& currentScene);
		template<typename T>
		void Register(const std::string& name)
		{
			std::cout << "Registering scenes: " << name << std::endl;
			// pass into a lambda function that creates an instance of typename T
			m_scenes.insert(std::make_pair(name, []() { return new T(); }));
		}
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		std::unique_ptr<Renderer> m_renderer;
		Scene*& m_currentScene;
		std::unordered_map<std::string, std::function<Scene*()>> m_scenes;
		ImVec4 m_clear_color;
	};
}
