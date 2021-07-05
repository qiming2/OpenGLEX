#include "SimpleModelScene.h"
#include "Common.h"

namespace Scene{
	SimpleModelScene::SimpleModelScene():
		model("res/Model/simple_model/backpack.obj"),
		shader("res/shaders/simple_model_vert.shader", "res/shaders/simple_model_frag.shader"),
		model_m(glm::mat4(1.0f))
	{
		
	}

	SimpleModelScene::~SimpleModelScene() {
	
	}

	void SimpleModelScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}

	void SimpleModelScene::OnRendering() {
		gl_renderer.Clear();
		shader.Bind();
		camera.SetViewProjectMat(&shader);
		shader.SetMat4fv("model", glm::value_ptr(model_m));
		model.Draw(shader);
	}

	void SimpleModelScene::OnImGuiRendering() {
	
	}
}