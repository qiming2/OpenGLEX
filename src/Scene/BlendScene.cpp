#include <map>
#include "BlendScene.h"
#include "Common.h"

namespace Scene{
	static std::vector<glm::vec3> positions = {
		glm::vec3(-1.5f,  0.0f, -0.48f),
		glm::vec3( 1.5f,  0.0f,  0.51f),
		glm::vec3( 0.0f,  0.0f,  0.7f),
		glm::vec3(-0.3f,  0.0f, -2.3f),
		glm::vec3( 0.5f,  0.0f, -0.6f)
	};
	BlendScene::BlendScene():
		shader("res/shaders/simple_model_vert.shader", "res/shaders/simple_model_frag.shader"),
		model_m(glm::mat4(1.0f)),
		cube(MeshType::Cube),
		quad(MeshType::Quad),
		floor(MeshType::Quad)
	{
		floor.ChangeTexture("res/Texture/se_1.jpg", 0);
		Texture tex("res/Texture/blending_transparent_window.png", GL_TEXTURE0, "texture_diffuse");

		// Since grass is a transparent object
		// at border we don't want it to interpolate
		// Thus change it to clamp to edge
		tex.Bind();
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
		tex.Unbind();
		quad.ChangeTexture(tex, 0);

		// Blend!! 
		glEnable(GL_BLEND);
		// How we want to blend objects
		// out_color = src.color.a * src.color + (1.0f - src.color.a) * des.color;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	BlendScene::~BlendScene() {
		shader.Delete();
		cube.Delete();
		quad.Delete();
		floor.Delete();
	}

	void BlendScene::OnUpdate(float deltaTime) {
		camera.processInput();
		
	}

	void BlendScene::OnRendering() {
		gl_renderer.Clear();
		shader.Bind();
		camera.SetViewProjectMat(&shader);
		
		///////////////////////////// Tips ////////////////////////////////
		// When needing to draw transparent objects, steps to draw objects:
		// 1. Opaque
		// 2. Sort transparent objects
		// 3. draw from furthest to the closest

		// followup question: what if objects intersect with each other?
		// this method does not take into account weirdly transformed
		// objects

		
		// drawing transparent object first can be interesting, which will
		// make the viewer seem to see through the object in the middle

	
		/////////////////////////////// Tips ////////////////////////////

		// Simple Face culling understanding is not complex
		// thus put it in BlendScene
		glEnable(GL_CULL_FACE);
		// Cull front face to see an interesting seeing through effect
		glCullFace(GL_FRONT);
		// Only render faces that are indexed in clockwise order
		glFrontFace(GL_CCW);



		model_m = glm::mat4(1.0f);
		model_m = glm::translate(model_m, glm::vec3(-1.5f,  0.01f, -1.0f));
		shader.SetMat4fv("model", glm::value_ptr(model_m));
		cube.Draw(shader);

		// For quads, we want to draw both faces
		glDisable(GL_CULL_FACE);
		model_m = glm::mat4(1.0f);
		model_m = glm::translate(model_m, glm::vec3(0.0f,  -0.5f, 0.0f));
		model_m = glm::scale(model_m, glm::vec3(5.0f, 5.0f, 5.0f));
		shader.SetMat4fv("model", model_m);
		floor.Draw(shader);


		// sort using std::map automatically sort by keys
		std::map<float, glm::vec3> sorted_map;
		for (int i = 0; i < positions.size(); i++) {
			float distance = glm::length(camera.pos - positions[i]);
			sorted_map.emplace(distance, positions[i]);
		}

		glm::mat4 per_quad(1.0f);
		model_m = glm::mat4(1.0f);
		model_m = glm::rotate(model_m, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		
		// draw from furthest to the closest
		// use reverse iterator
		std::map<float, glm::vec3>::reverse_iterator it = sorted_map.rbegin();
		for (; it != sorted_map.rend(); it++) {
			per_quad = glm::translate(glm::mat4(1.0f), it->second) * model_m;
			shader.SetMat4fv("model", glm::value_ptr(per_quad));
			quad.Draw(shader);
		}
		
	}

	void BlendScene::OnImGuiRendering() {
		ImGui::SliderFloat("camera far plane", &camera.far, 0.0f, 10000.0f);
		ImGui::SliderFloat("camera speed", &camera.cameraSpeed, 0.0f, 1000.0f);
	}
}