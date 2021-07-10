#include "BlendScene.h"
#include "Common.h"

namespace Scene{
	BlendScene::BlendScene():
		shader("res/shaders/simple_model_vert.shader", "res/shaders/simple_model_frag.shader"),
		model_m(glm::mat4(1.0f)),
		cube(MeshType::Cube),
		quad(MeshType::Quad)
	{

	}

	BlendScene::~BlendScene() {
		shader.Delete();
		cube.Delete();
		quad.Delete();
	}

	void BlendScene::OnUpdate(float deltaTime) {
		camera.processInput();
		
	}

	void BlendScene::OnRendering() {
		gl_renderer.Clear();
		shader.Bind();
		camera.SetViewProjectMat(&shader);
		

		model_m = glm::mat4(1.0f);
		shader.SetMat4fv("model", glm::value_ptr(model_m));

		cube.Draw(shader);

		model_m = glm::mat4(1.0f);
		model_m = glm::translate(model_m, glm::vec3(0.0f, -0.51f, 0.0f));
		model_m = glm::scale(model_m, glm::vec3(5.0f, 5.0f, 5.0f));
		shader.SetMat4fv("model", glm::value_ptr(model_m));
		quad.Draw(shader);
	}

	void BlendScene::OnImGuiRendering() {
		ImGui::SliderFloat("camera far plane", &camera.far, 0.0f, 10000.0f);  
	}
}