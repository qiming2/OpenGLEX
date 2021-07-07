#include "SimpleModelScene.h"
#include "Common.h"

namespace Scene{
	float radians = 0.0;
	float timeToRotateACycle = 3.0f;
	float radiansPerSec = 0.0;
	SimpleModelScene::SimpleModelScene():
		model("res/Model/simple_model/backpack.obj", false),
		shader("res/shaders/simple_model_vert.shader", "res/shaders/simple_model_frag.shader"),
		model_m(glm::mat4(1.0f)),
		barbara("res/Model/Genshin/barbara/barbara.pmx")
	{
		radiansPerSec = 2 * M_PI / timeToRotateACycle; 
	}

	SimpleModelScene::~SimpleModelScene() {
		model.Delete();
		barbara.Delete();
		shader.Delete();
	}

	void SimpleModelScene::OnUpdate(float deltaTime) {
		camera.processInput();
		radians += radiansPerSec * DeltaTime;
	}

	void SimpleModelScene::OnRendering() {
		gl_renderer.Clear();
		shader.Bind();
		camera.SetViewProjectMat(&shader);
		

		model_m = glm::mat4(1.0f);
		shader.SetVec3fv("viewPos", camera.pos);
		shader.SetMat4fv("model", glm::value_ptr(model_m));

		model.Draw(shader);

		model_m = glm::mat4(1.0f);
		model_m = glm::translate(model_m, glm::vec3(5.0f, -10.0f, 0.0f));
		model_m = glm::rotate(model_m, radians, glm::vec3(0.0f, 1.0f, 0.0f));
		model_m = glm::scale(model_m, glm::vec3(0.7f, 0.7f, 0.7f));
		shader.SetMat4fv("model", glm::value_ptr(model_m));
		barbara.Draw(shader);
	}

	void SimpleModelScene::OnImGuiRendering() {
	
	}
}