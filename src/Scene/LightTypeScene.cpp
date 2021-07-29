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
	static bool pointLight = false;
	static bool directionalLight = false;
	static bool spotLight = false;
	glm::vec3 pointLightPositions[] = {
		glm::vec3( 0.7f,  0.2f,  2.0f),
		glm::vec3( 2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3( 0.0f,  0.0f, -3.0f)
	}; 
	LightTypeScene::LightTypeScene():
		lightColor(glm::vec3(1.0f, 1.0f ,1.0f))
	{
		Texture::SetFlip(true);
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
		specularTexture = new Texture("res/Texture/starsky.jpeg", textureSlot++);

		// Use shader class to load
		phong = new m_Shader("res/shaders/light_type_vert.shader", "res/shaders/light_type_frag.shader");

		light_cube = new m_Shader("res/shaders/light_cube_vert.shader", "res/shaders/light_cube_frag.shader");

		// Fps Camera to render a movable scene
		//camera = CameraFps();

		// Create a new renderer
		renderer = new Renderer();

		// Enable depth for realistic view
		glm::vec3 cubePositions1[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		for (int i = 0; i < sizeof(cubePositions)/sizeof(glm::vec3); i++) {
			cubePositions[i] = cubePositions1[i];
		}
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
		//model = glm::mat4(1.0f);
		////model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, (float) glfwGetTime() * 1.5f, glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)));


		model_light = glm::mat4(1.0f);
		//model_light = glm::rotate(model_light, (float) glfwGetTime() * 1.5f, glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)));

		model_light = glm::translate(model_light, glm::vec3(2.0f, 2.0f, 2.0f));
		model_light = glm::scale(model_light, glm::vec3(0.1f, 0.1f, 0.1f));
	}

	void LightTypeScene::OnRendering() {
		renderer->Clear();
		// Change background to dark color
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		// Send diffuse texture
		

		// render main cube
		glBindVertexArray(va_id);
		//glBindBuffer(GL_ARRAY_BUFFER, vb_id);
		
		if (pointLight) {
			light_cube->Bind();
			camera.SetViewProjectMat(light_cube);
			light_cube->SetVec3fv("lightColor", glm::value_ptr(lightColor));
			for (int i = 0; i < sizeof(pointLightPositions)/sizeof(glm::vec3); i++) {
				model_light = glm::mat4(1.0f);
				model_light = glm::translate(model_light, pointLightPositions[i]);
				model_light = glm::scale(model_light, glm::vec3(0.1f, 0.1f ,0.1f));
				light_cube->SetMat4fv("model", glm::value_ptr(model_light));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		
		

		diffuseTexture->Bind();
		specularTexture->Bind();
		phong->Bind();
		// Material
		phong->SetInt("material.diffuse", 0);
		phong->SetInt("material.specular", 1);
		phong->SetVec3fv("viewPos", camera.pos);
		phong->SetFloat("shininess", 32.0f);
		// Directional light

		phong->SetInt("useDirectionalLight", directionalLight);
		phong->SetVec3fv("light.direction", glm::vec3(0.0f, 0.0f, -1.0f));
		phong->SetVec3fv("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		phong->SetVec3fv("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		phong->SetVec3fv("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		

		// point light
		for (int i = 0; i < sizeof(pointLightPositions)/sizeof(glm::vec3); i++) {
			phong->SetInt("usePointLight", pointLight);
			std::string number = std::to_string(i);
			phong->SetVec3fv("pLight[" + number + "].pos", pointLightPositions[i]);
			phong->SetVec3fv("pLight[" + number + "].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			phong->SetVec3fv("pLight[" + number + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			phong->SetVec3fv("pLight[" + number + "].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
			// values of atten depend on the distance coverage of the point light we are setting
			// weblink: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
			phong->SetFloat("pLight[" + number + "].constantAtten", 1.0f);
			phong->SetFloat("pLight[" + number + "].linearAtten", 0.09f);
			phong->SetFloat("pLight[" + number + "].quadraticAtten", 0.032f);
		}
		

		// Spot light
		phong->SetInt("useSpotLight", spotLight);
		phong->SetVec3fv("sLight.pos", camera.pos);
		phong->SetVec3fv("sLight.direction", camera.target - camera.pos);
		phong->SetFloat("sLight.cutoff", glm::cos(glm::radians(12.5f)));
		phong->SetFloat("sLight.outerCutoff", glm::cos(glm::radians(17.5f)));
		phong->SetVec3fv("sLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		phong->SetVec3fv("sLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		phong->SetVec3fv("sLight.diffuse", glm::vec3(1.5f, 1.5f, 1.5f));
		phong->SetFloat("sLight.constantAtten", 1.0f);
		phong->SetFloat("sLight.linearAtten", 0.09f);
		phong->SetFloat("sLight.quadraticAtten", 0.032f);


		camera.SetViewProjectMat(phong);
		float angle;
		for (int i = 0; i < 10; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			angle = i * 20.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			phong->SetMat4fv("model", glm::value_ptr(model));
			// very important step, otherwise normals are all incorrect
			phong->SetMat3fv("normalM", glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(model)))));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		

		// render light cube
		

	}

	void LightTypeScene::OnImGuiRendering() {
		glm::vec3 posLight = glm::vec3(camera.getView() * model_light * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec3 posObj = glm::vec3(camera.getView() * model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		ImGui::Text("Pos light %.1f %.1f %.1f", posLight.x, posLight.y, posLight.z);
		ImGui::Text("Pos Obj  %.1f %.1f %.1f", posObj.x, posObj.y, posObj.z);
		ImGui::Text("Pos Camera %.1f %.1f %.1f", camera.pos.x, camera.pos.y, camera.pos.z);
		glm::vec3 cameraDir = camera.target - camera.pos;
		ImGui::Text("Camera direction %.1f %.1f %.1f", cameraDir.x, cameraDir.y, cameraDir.z);
		ImGui::Checkbox("Directional Light", &directionalLight);
		ImGui::Checkbox("PointLight", &pointLight);
		ImGui::Checkbox("SpotLight", &spotLight);
	}

}