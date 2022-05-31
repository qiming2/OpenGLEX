#include <iostream>
#include <ostream>
#include "DepthScene.h"
#include "GeometryUtil.h"


namespace Scene {


	// Can learn more about z-fighting
	DepthScene::DepthScene():
		model_m(1.0f),
		model("res/Model/Sponza/sponza.mobj"),
		shader("res/shaders/depth_vert.shader", "res/shaders/depth_frag.shader")
	{
		std::vector<float> plane = CreatePlane();
		va = new VertexArray();
		va->Bind();
		vb = new VertexBuffer(plane.data(), sizeof(float) * plane.size());
		vb->Bind();
		std::vector<unsigned int> indices = {
			0, 1, 2,
			3, 4, 5
		};
		ib = new IndexBuffer(indices.data(), indices.size());
		ib->Bind();
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		shader.Bind();
		va->AddBuffer(*vb, layout);
		
	}

	DepthScene::~DepthScene() {
		model.Delete();
		shader.Delete();
		va->Delete();
		vb->Delete();
		ib->Delete();
		delete va;
		delete vb;
		delete ib;
	}

	void DepthScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}

	void DepthScene::OnImGuiRendering() {
		ImGui::SliderFloat("camera far plane", &camera.far, 0.0f, 10000.0f);
		ImGui::SliderFloat("camera speed", &camera.cameraSpeed, 0.0f, 1000.0f);
	}
	
	void DepthScene::OnRendering() {
		// Non-linear depth val, so all most all the objects have depth close to 1.0
		// more precision close to near plane, low precision at far plane
		gl_renderer.Clear();
		shader.Bind();
		shader.SetFloat("far", camera.far);
		shader.SetFloat("near", camera.near);
		camera.SetViewProjectMat(&shader);
		/*model_m = glm::mat4(1.0f);
		model_m = glm::scale(model_m, glm::vec3(1.0f, 1.0f, 100.0f));
		shader.SetMat4fv("model", model_m);
		gl_renderer.Draw(*va, *ib);*/
		
		model_m = glm::mat4(1.0);
		glm::vec4 temp = camera.getProjection() * camera.getView() * model_m * glm::vec4(1.0, 1.0, 1.0, 1.0);
		//std::cout << temp << std::endl;
		shader.SetMat4fv("model", model_m);
		model.Draw(shader);
	}

	
}