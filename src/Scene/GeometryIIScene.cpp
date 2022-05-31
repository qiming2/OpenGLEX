#include "GeometryIIScene.h"
#include "Common.h"

// explode experiment with geometry shader adding vertices, changing gl_position, etc
// display normals at vertices position
namespace Scene {
	static bool explode = false;
	static bool normal = false;
	GeometryIIScene::GeometryIIScene():
		ayaka("res/Model/Genshin/ayaka/ayaka.pmx"),
		shader("res/shaders/geometryII_vert.shader", "res/shaders/geometryII_frag.shader", "res/shaders/geometryII_geom.shader"),
		model(glm::mat4(1.0f)),
		ub(2 * sizeof(glm::mat4)),
		cube(MeshType::Cube),
		normal_shader("res/shaders/geometry_normal_vert.shader", "res/shaders/geometry_normal_frag.shader", "res/shaders/geometry_normal_geom.shader")
	{
  		// Set up uniform binding point so that many shaders can use the shared
		// 
		ub.SetbindingPoint(0, 1, 2 * sizeof(glm::mat4));
		shader.SetUniformBlock("Camera", 1);
		normal_shader.SetUniformBlock("Camera", 1);
	}

	GeometryIIScene::~GeometryIIScene() {
		ayaka.Delete();
		shader.Delete();
		cube.Delete();
	}

	void GeometryIIScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}

	void GeometryIIScene::OnImGuiRendering() {
		camera.OnImGuiRendering();
		ImGui::Checkbox("Explode", &explode);
		ImGui::Checkbox("Show Normal", &normal);
	}

	void GeometryIIScene::OnRendering() {
		gl_renderer.Clear();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, -15.0f));
		//model_m = glm::rotate(model_m, radians, glm::vec3(0.0f, 1.0f, 0.0f));
		shader.Bind();
		ub.Subdata(0, sizeof(glm::mat4), (void*) glm::value_ptr(camera.getProjection()));
		ub.Subdata(sizeof(glm::mat4), sizeof(glm::mat4), (void*)glm::value_ptr(camera.getView()));
		if (explode)
			shader.SetFloat("time", (float)glfwGetTime());
		else
			shader.SetFloat("time", 1.5f * M_PI);
		shader.SetMat4fv("model", model);
		ayaka.Draw(shader);
		if (normal) {
			normal_shader.Bind();
			normal_shader.SetMat4fv("model", model);
			if (explode)
				normal_shader.SetFloat("time", (float)glfwGetTime());
			else
				normal_shader.SetFloat("time", 1.5f * M_PI);
			ayaka.Draw(normal_shader);
		}
		
		shader.Bind();
		if (explode)
			shader.SetFloat("time", (float)glfwGetTime());
		else
			shader.SetFloat("time", 1.5f * M_PI);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f));
		shader.SetMat4fv("model", model);
		cube.Draw(shader);
		if (normal) {
			normal_shader.Bind();
			normal_shader.SetMat4fv("model", model);
			if (explode)
				normal_shader.SetFloat("time", (float)glfwGetTime());
			else
				normal_shader.SetFloat("time", 1.5f * M_PI);
			cube.Draw(normal_shader);
		}	
	}
}