#pragma once
#include "Scene.h"

namespace Scene {
	class InstanceScene : public Scene
	{
	public:
		InstanceScene();
		~InstanceScene();

		void OnUpdate(float deltaTime) override;
		void OnImGuiRendering() override;
		void OnRendering() override;
	private:
		m_Shader shader;

		// simple quad mesh info
		VertexArray s_va;
		VertexBuffer s_vb;
		VertexBuffer s_vb1;
		IndexBuffer s_ib;
	};

}


