#include "MSAAScene.h"
#include "Common.h"
#include "GL/glew.h"

namespace Scene {
	static int samples = 4;
	static bool MSAA = false;
	MSAAScene::MSAAScene():
		cube(MeshType::Cube),
		shader("res/shaders/MSAA_vert.shader", "res/shaders/MSAA_frag.shader"),
		screen_shaderMS("res/shaders/screen_quad_ms_vert.shader", "res/shaders/screen_quad_ms_frag.shader"),
		model(1.0f)
	{

		glGenFramebuffers(1, &msfb);
		glBindFramebuffer(GL_FRAMEBUFFER, msfb);
		currentfb = 0;
		// multisampled texture
		glGenTextures(1, &mstex);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mstex);
		// specify sample count, width, height, internal format
		// the last one indicates whether we want all samples being sampled
		// at the same locations for each pixel
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, Width, Height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mstex, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);

		// alloction depth and stencil buffer
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, Width, Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		// Check if framebuffer is complete and unbind it
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "FRAMEBUFFER:NOT COMPLETE" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
		screen_shaderMS.Bind();
		screen_shaderMS.SetInt("texture_diffuse1MS", 0);

	}

	MSAAScene::~MSAAScene()
	{
	}

	void MSAAScene::OnUpdate(float deltaTime)
	{
		camera.processInput();
	}

	void MSAAScene::OnImGuiRendering()
	{
		camera.OnImGuiRendering();
		ImGui::Checkbox("MSAA", &MSAA);
	}

	void MSAAScene::OnRendering()
	{
		// bind ms framebuffer and draw the scene first
		if (MSAA) {
			currentfb = msfb;
		} else {
			currentfb = 0;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, currentfb);
		gl_renderer.Clear();
		shader.Bind();
		camera.SetViewProjectMat(shader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -1.0f));

		model = glm::rotate(model, (float)(0.25f * M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		shader.SetMat4fv("model", model);
		cube.Draw(shader);
		
		// if we just want to render a msaa scene
		// without sampling from it, we can use
		// glBlitFramebuffer()
		/*if (currentfb) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, msfb);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}*/

		// custom anti-aliasing can be achieved by sampling
		// the multisample texture in our own shader
		// currently, we just replicate msaa samples = 4
		// which we basically add up all sample colors
		// and divide it by 4 to get the average color
		// for that pixel
		if (MSAA) {
			glDisable(GL_DEPTH_TEST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			gl_renderer.Clear();
			screen_va.Bind();
			screen_shaderMS.Bind();
			screen_shaderMS.SetInt("samples", samples);
			screen_shaderMS.SetInt("width", Width);
			screen_shaderMS.SetInt("height", Height);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mstex);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);
		}
		
		

		
	}

}

