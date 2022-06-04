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
	};

}

