#include <vector>
#include "LightTypeScene.h"
#include "Common.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GeometryUtil.h"
#include "Renderer.h"
#include "Texture.h"
#include "m_Shader.h"


// Gonna practice write all the rendering without using
// Engine classes

namespace Scene {
	LightTypeScene::LightTypeScene():
		lightColor(glm::vec3(1.0f, 1.0f ,1.0f))
	{
		std::vector<float> vertices = CreateCubeWithNormal();

		// sizeof could not determine the size of the array since
		// it is treated as a pointer
		float* ver= vertices.data();
		
		// VertexArray
		GLCall(glGenVertexArrays(1, &va_id));
		GLCall(glBindVertexArray(va_id));

		// Vertex Buffer
		GLCall(glGenBuffers(1, &vb_id));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb_id));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), ver, GL_STATIC_DRAW));

		// Vertex attribute stride is often the size of all attributes in one vertex
		unsigned int stride = 8 * sizeof(GL_FLOAT);

		// offset is a number!!! It means how many bytes away from the start of the pointer are to get the first type of the specified attribute

		// Position: has three elements
		GLCall(glEnableVertexAttribArray(0))
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0));
		// Normal
		GLCall(glEnableVertexAttribArray(1))
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float))));

		// Texture
		GLCall(glEnableVertexAttribArray(2))
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*) (6 * sizeof(float))));

		// Use texture class since we can stb image_loading library
		GLenum textureSlot = GL_TEXTURE0;
		diffuseTexture = new Texture("res/Texture/starsky.jpeg", textureSlot++);
		specularTexture = new Texture("res/Texture/lighting_maps_specular_color.png", textureSlot++);

		// Use shader class to load
		phong = new m_Shader("res/shaders/light_type_vert.shader", "res/shaders/light_type_frag.shader");

		light_cube = new m_Shader("res/shaders/light_cube_vert.shader", "res/shaders/light_cube_frag.shader");

		// Fps Camera to render a movable scene
		//camera = CameraFps();

		// Create a new renderer
		renderer = new Renderer();

		// Enable depth for realistic view
		glEnable(GL_DEPTH_TEST);
	}

	LightTypeScene::~LightTypeScene() {
		delete diffuseTexture;
		delete specularTexture;
		delete phong;
		delete light_cube;
		//delete camera;
		delete renderer;
		GLCall(glDeleteVertexArrays(1, &va_id));
		GLCall(glDeleteBuffers(1, &vb_id));
	}

	void LightTypeScene::OnUpdate(float deltaTime) {
		camera.processInput();
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, (float) glfwGetTime() * 1.5f, glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)));


		model_light = glm::mat4(1.0f);
		//model_light = glm::rotate(model_light, (float) glfwGetTime() * 1.5f, glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)));

		model_light = glm::translate(model_light, glm::vec3(2.0f, 2.0f, 2.0f));
		model_light = glm::scale(model_light, glm::vec3(0.1f, 0.1f, 0.1f));
	}

	void LightTypeScene::OnRendering() {
		renderer->Clear();

		// render main cube
		glBindVertexArray(va_id);
		//glBindBuffer(GL_ARRAY_BUFFER, vb_id);
		light_cube->Bind();
		camera.SetViewProjectMat(light_cube);
		light_cube->SetMat4fv("model", glm::value_ptr(model_light));
		light_cube->SetVec3fv("lightColor", glm::value_ptr(lightColor));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		phong->Bind();
		phong->SetMat4fv("normalM", glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(model)))));
		camera.SetViewProjectMat(phong);
		phong->SetMat4fv("model", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// render light cube
		

	}

	void LightTypeScene::OnImGuiRendering() {
		glm::vec3 posLight = glm::vec3(camera.getView() * model_light * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec3 posObj = glm::vec3(camera.getView() * model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		ImGui::Text("Pos light %.1f %.1f %.1f", posLight.x, posLight.y, posLight.z);
		ImGui::Text("Pos Obj  %.1f %.1f %.1f", posObj.x, posObj.y, posObj.z);
		ImGui::Text("Pos Camera %.1f %.1f %.1f", camera.pos.x, camera.pos.y, camera.pos.z);
	}

}