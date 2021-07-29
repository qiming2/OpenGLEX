#include "InstanceIIScene.h"
#include "VertexBufferLayout.h"
#include "Common.h"
#include "Mesh.h"

namespace Scene {
	static std::vector<std::string> concatFacePath(const std::string path) {
		Texture::SetFlip(false);
		return { path + "right.png", path + "left.png", path + "top.png", path + "bottom.png", path + "front.png", path + "back.png" };
	}
	static float radius = 500.0f;
	static float offset = 200.0f;
	static float heightOffset = 50.0f;
	static float scaleMin = 0.5f;
	static float scaleMax = 0.7f;
	// define a static rotation axis for rock
	static glm::vec3 axis = glm::vec3(1.0f, 1.0f, 1.0f);
	static const int amount = 200000;
	static float scaling = 0.1f;
	// Simple demonstration of how to use instance drawing
	InstanceIIScene::InstanceIIScene() :
		shader("res/shaders/simple_model_vert.shader", "res/shaders/simple_model_frag.shader"),
		rock("res/Model/rock/rock.mobj"),
		asteroid("res/Model/planet/planet.mobj"),
		model(1.0f),
		instanceS("res/shaders/instance_model_vert.shader", "res/shaders/instance_model_frag.shader"),
		cm(concatFacePath("res/CubeMap/space/"), GL_TEXTURE0),
		cm_shader("res/shaders/cube_map_vert.shader", "res/shaders/cube_map_frag.shader"),
		cube(MeshType::Cube)
	{
		cm_shader.Bind();
		cm_shader.SetInt("cubeMap", 0);
		// compute model m at the start for all the rocks
		float random_dis;
		float x;
		float y;
		float z;
		float angle;
		float scaleRange = scaleMax - scaleMin;
		glm::mat4* models = new glm::mat4[amount];
		for (int i = 0; i < amount; i++) {
			angle = (float)i / amount * M_PI * 2;
			model = glm::mat4(1.0f);
			// for more precions, we need to multiply by 100 and divide by 100.0f in the end
			// range from [-defined_offset, defined_offset)

			// translation
			random_dis = (rand() % (int)(offset * 2 * 100)) / 100.0f - offset;
			x = cos(angle) * (radius + random_dis);
			random_dis = (rand() % (int)(heightOffset * 2 * 100)) / 100.0f - heightOffset;
			y = random_dis;
			random_dis = (rand() % (int)(offset * 2 * 100)) / 100.0f - offset;
			z = sin(angle) * (radius + random_dis);
			model = glm::translate(model, glm::vec3(x, y, z));

			// rotation
			random_dis = (rand() % (int)(2 * M_PI * 100)) / 100.0f;
			model = glm::rotate(model, random_dis, axis);

			// scale
			random_dis = (rand() % (int)(scaleRange * 100)) / 100.0f + scaleMin;
			model = glm::scale(model, glm::vec3(random_dis, random_dis, random_dis));

			models[i] = model;
		}

		// for each mesh or va, we need to add the mat4 vertex attribute
		
		// set mat4 as vertex attribute is different from setting up vertex attribute
		// for other types
		
		// set up mat4 model buffer
		vb.ReMap(&models[0], sizeof(glm::mat4) * amount);
		vb.Bind();
		std::vector<Mesh>& meshes = rock.GetMeshes();
		unsigned int size = sizeof(glm::vec4);
		unsigned int mat_t = sizeof(glm::mat4);
		for (int i = 0; i < meshes.size(); i++) {
			VertexArray& va = meshes[i].GetVa();
			va.Bind();
			// set mat4 vertex attribute manually for practicing purpose
			// since I know 0 - 4 locations have been used

			// Since we are setting vertex attrib for a mat4
			// opengl wants us to specify mat4 with 4 vec4
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mat_t, (void*)0);
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mat_t, (void*)(1 * size));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, mat_t, (void*)(2 * size));
			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, mat_t, (void*)(3 * size));

			// Very important for instanced drawing if we want this attrib to change
			// per instance or per two instances, so on

			// Update vertex attibute at location layout location 5, 6, 7, 8 once per instance drawing
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);
			va.Unbind();
		}
		delete[] models;
	}

	InstanceIIScene::~InstanceIIScene() {
		shader.Delete();
		instanceS.Delete();
		vb.Delete();
		rock.Delete();
		asteroid.Delete();
		cm.Delete();
		cm_shader.Delete();
		cube.Delete();
	}

	void InstanceIIScene::OnUpdate(float deltaTime) {
		camera.processInput();

	}

	void InstanceIIScene::OnImGuiRendering() {
		camera.OnImGuiRendering();

	}

	void InstanceIIScene::OnRendering() {
		gl_renderer.Clear();
		shader.Bind();
		camera.SetViewProjectMat(shader);
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * scaling, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(75.0f, 75.0f, 75.0f));
		shader.SetMat4fv("model", model);
		asteroid.Draw(shader);

		
		// make the rock ring rotate over time;
		instanceS.Bind();
		camera.SetViewProjectMat(instanceS);
		model = glm::mat4(1.0f);

		// since rotating origin is (0.0, 0.0, 0.0)
		// we don't need to translate afterwards
		// if we want to rotate around an object
		// we need to translate with the parent
		// change translation here can achieve the effect
		// model = glm::translate(model, glm::vec3(100.0f, 0.0f, 0.0f));
		
		model = glm::rotate(model, (float)glfwGetTime() * scaling, glm::vec3(0.0f, 1.0f, 0.0f));
		instanceS.SetMat4fv("model", model);
		rock.DrawInstances(instanceS, amount);

		// Draw a skybox
		glDepthFunc(GL_LEQUAL);
		// disable write to depth buffer
		glDepthMask(GL_FALSE);
		cm_shader.Bind();
		cm.Bind();
		cm_shader.SetMat4fv("projection", camera.getPojection());
		// don't let skybox rotate
		cm_shader.SetMat4fv("view", glm::mat4(glm::mat3(camera.getView())));
		model = glm::mat4(1.0f);
		cm_shader.SetMat4fv("model", model);
		cube.Draw(cm_shader);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
}