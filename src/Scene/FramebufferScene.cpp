#include "FramebufferScene.h"
#include "Common.h"

namespace Scene {
	static void frame_buffer_callback(GLFWwindow* window, int Width, int height);
	bool sizeChanged = false;
	static std::vector<glm::vec3> positions = {
		glm::vec3(-1.5f,  0.0f, -0.48f),
		glm::vec3( 1.5f,  0.0f,  0.51f),
		glm::vec3( 0.0f,  0.0f,  0.7f),
		glm::vec3(-0.3f,  0.0f, -2.3f),
		glm::vec3( 0.5f,  0.0f, -0.6f)
	};
	FramebufferScene::FramebufferScene():
		normalShader("res/shaders/simple_model_vert.shader", "res/shaders/simple_model_frag.shader"),
		screenShader("res/shaders/screen_quad_vert.shader", "res/shaders/screen_quad_frag.shader"),
		cube(MeshType::Cube),
		floor(MeshType::Quad),
		kernel_type(1)
	{
		glfwSetFramebufferSizeCallback(Window, frame_buffer_callback);
		cube.ChangeTexture("res/Texture/container.jpg", 0);
		floor.ChangeTexture("res/Texture/starsky.jpeg", 0);
		
		// Screen quad set up
		screen_va.Bind();
		std::vector<float> vertices = CreateScreenQuad();
		screen_vb.ReMap(vertices.data(), vertices.size() * sizeof(float));
		std::vector<unsigned int> indices = {
			0, 1, 2,
			3, 4, 5
		};
		screen_ib.ReMap(indices.data(), indices.size());
		
		VertexBufferLayout layout;
		// NDC coords
		layout.Push<float>(2);

		// Tex coords
		layout.Push<float>(2);
		screen_va.AddBuffer(screen_vb, layout);
		screenShader.Bind();
		screenShader.SetInt("texture_diffuse1", 0);
	}

	FramebufferScene::~FramebufferScene() {
		fb.Delete();
		normalShader.Delete();
		screenShader.Delete();
		cube.Delete();
		floor.Delete();
		screen_va.Delete();
		screen_vb.Delete();
		screen_ib.Delete();
	}

	void FramebufferScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}

	void FramebufferScene::OnImGuiRendering() {
		camera.OnImGuiRendering();
		if (ImGui::Button("Blur")) {
			kernel_type = 1 << 0;
		} else if (ImGui::Button("Normal")) {
			kernel_type = 0;
		}

		if (ImGui::Button("Edge")) {
			kernel_type = 1 << 1;
		}

		if (ImGui::Button("Sharpen")) {
			kernel_type = 1 << 2;
		}
	}

	void FramebufferScene::OnRendering() {

		// First pass, render to the framebuffer

		///////////////////////////////// WARNING ///////////////////
		// Be sure to bind framebuffer first and then clear!
		if (sizeChanged) {
			sizeChanged = false;
			fb.Resize(Width, Height);
		}
		fb.Bind();
		gl_renderer.Clear();
		glEnable(GL_DEPTH_TEST);
		normalShader.Bind();
		camera.SetViewProjectMat(normalShader);
		for (int i = 0; i < positions.size(); i++) {
			model_m = glm::translate(glm::mat4(1.0f), positions[i]);
			normalShader.SetMat4fv("model", model_m);
			cube.Draw(normalShader);
		}

		model_m = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.6f, 0.0f));
		model_m = glm::scale(model_m, glm::vec3(10.0f, 10.0f, 10.0f));
		normalShader.SetMat4fv("model", model_m);
		floor.Draw(normalShader);
		fb.Unbind();
		// Very important since we always want to
		// keep screen quad drewn on screen
		glDisable(GL_DEPTH_TEST);
		// Second pass, render to the screen
		// We need to clear again since this is
		// a different framebuffer
		gl_renderer.Clear();
		screenShader.Bind();
		screenShader.SetInt("type", kernel_type);
		screenShader.SetFloat("width", Width);
		screenShader.SetFloat("height", Height);
		fb.BindTexture(GL_TEXTURE0);
		screen_va.Bind();
		gl_renderer.Draw(screen_va, screen_ib);
	}

	static void frame_buffer_callback(GLFWwindow* window, int width, int height)
	{
		Width = width;
		Height = height;
		sizeChanged = true;
		glViewport(0, 0, width, height);
	}
}