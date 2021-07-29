#include "InstanceScene.h"
#include "VertexBufferLayout.h"
#include "Common.h"

namespace Scene {

	// Simple demonstration of how to use instance drawing
	InstanceScene::InstanceScene():
		shader("res/shaders/instance_vert.shader", "res/shaders/instance_frag.shader")
	{
		static float vertices[] = 
		{
			-0.025, 0.025, 1.0, 0.0, 0.0,
			-0.025, -0.025, 0.0, 1.0, 0.0,
			0.025, 0.025, 0.0, 0.0, 1.0,
			0.025, -0.025, 0.0, 0.0, 1.0
		};
		static unsigned int indices[] =
		{
			0, 1, 2,
			2, 1, 3
		};
		s_va.Bind();
		s_vb.ReMap(vertices, sizeof(vertices));
		s_ib.ReMap(indices, sizeof(indices) / sizeof(indices[0]));
		//std::cout << "size of: " << sizeof(vertices) << std::endl;
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(3);

		// here we use glVertexAttributeDivisor
		// to take advantage of updating certain
		// vertex info per instance
		float x_offset = -0.9f;
		float y_offset = 0.9f;
		glm::vec2 offset[19 * 19];

		for (int i = 0; i < 19; i++) {
			for (int j = 0; j < 19; j++) {
				glm::vec2& ref = offset[i * 19 + j];
				ref.x = x_offset + i * 0.1;
				ref.y = y_offset - j * 0.1;
			}
		}

		s_vb1.ReMap(&offset[0], sizeof(offset));
		VertexBufferLayout layout1;
		layout1.Push<float>(2);

		s_va.AddBuffer(s_vb, layout);
		s_va.AddBuffer(s_vb1, layout1);
		// vertex attribute at location 2 would
		// only update once per instance
		glVertexAttribDivisor(2, 1);
		s_va.Unbind();
	}

	InstanceScene::~InstanceScene() {
		shader.Delete();
		s_va.Delete();
		s_vb.Delete();
		s_ib.Delete();
	}

	void InstanceScene::OnUpdate(float deltaTime) {


	}

	void InstanceScene::OnImGuiRendering() {


	}

	void InstanceScene::OnRendering() {
		gl_renderer.Clear();
		shader.Bind();
		s_ib.Bind();
		s_va.Bind();

		// 2,000,000 * 2 triangles draw can still reach 15 fps, pretty insane
		// glDrawElementsInstanced(GL_TRIANGLES, s_ib.GetCount(), GL_UNSIGNED_INT, NULL, 2000000);
		glDrawElementsInstanced(GL_TRIANGLES, s_ib.GetCount(), GL_UNSIGNED_INT, NULL, 19*19);
	}
}