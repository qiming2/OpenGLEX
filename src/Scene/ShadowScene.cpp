#include "ShadowScene.h"
#include "Common.h"

namespace Scene {

	const unsigned int ss_map_w = 1024;
	const unsigned int ss_map_h = 1024;
	// directional light
	glm::vec3 lightDir = glm::normalize(glm::vec3(0.5f, -0.5f, 0.5f));
	glm::vec3 lightPos = glm::vec3(0.0f) - lightDir * 10.0f;
	float near_plane = 1.0f;
	float far_plane = 100.0f;
	ShadowScene::ShadowScene() :
		cube(MeshType::Cube),
		plane(MeshType::Quad),
		shadowP("res/shaders/shadow_pass_vert.shader", "res/shaders/shadow_pass_frag.shader"),
		normalS("res/shaders/shadow_light_vert.shader", "res/shaders/shadow_light_frag.shader"),
		screenQ(MeshType::Quad)
	{
		// prepare directional light view depth map
		glGenFramebuffers(1, &ddfb);
		
		// depth map
		glGenTextures(1, &ddtex);
		glBindTexture(GL_TEXTURE_2D, ddtex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ss_map_w, ss_map_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		// areas outside specified light space are clamped to be 1.0
		// which means they are all lit
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// Bind texture to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, ddfb);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ddtex, 0);
		// Since we are only collecting data not drawing to the screen, we need
		// tell opengl not to draw to or read from any framebuffer
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		config();
		shadowP.Bind();
		shadowP.SetMat4fv("lightProjectView", lightProjectView);
	}

	ShadowScene::~ShadowScene() {
		cube.Delete();
		plane.Delete();
		screenQ.Delete();
		shadowP.Delete();
		normalS.Delete();
		glDeleteTextures(1, &ddtex);
		glDeleteFramebuffers(1, &ddfb);
	}

	void ShadowScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}

	void ShadowScene::OnImGuiRendering() {
		camera.OnImGuiRendering();
	}

	void ShadowScene::OnRendering() {
		
		// Cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(-0.2, 1.0f, -0.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		// Plane
		planeM = glm::mat4(1.0f);
		planeM = glm::rotate(planeM, (float)M_PI * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		planeM = glm::scale(planeM, glm::vec3(100.0f, 100.0f, 100.0f));
		// Shadow Pass
		glViewport(0, 0, ss_map_w, ss_map_h);
		glBindFramebuffer(GL_FRAMEBUFFER, ddfb);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowP.Bind();
		shadowP.SetMat4fv("model", model);
		cube.Draw(shadowP);
		shadowP.SetMat4fv("model", planeM);
		plane.Draw(shadowP);

		// Render Scene 
		glViewport(0, 0, Width, Height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		gl_renderer.Clear();
		normalS.Bind();
		normalS.SetVec3fv("dir", lightDir);
		normalS.SetVec3fv("viewPos", camera.pos);
		normalS.SetInt("texture1", 2);
		camera.SetViewProjectMat(normalS);
		// LightProjectView for testing whether the pixel is
		// in shadow
		normalS.SetMat4fv("lightProjectView", lightProjectView);
		// Always activate texture slot first
		// then bind texture to that texture slot
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, ddtex);
		normalS.SetMat4fv("model", model);
		cube.Draw(normalS);
		normalS.SetMat4fv("model", planeM);
		plane.Draw(normalS);
		
	}

	void ShadowScene::config() {
		// light view and projection matrix
		// directional light applicable range
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		// light view
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightProjectView = lightProjection * lightView;

	}
}