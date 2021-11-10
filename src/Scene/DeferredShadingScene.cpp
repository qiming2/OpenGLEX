#include "DeferredShadingScene.h"
namespace Scene {
	static const glm::vec3 pColor = { 20.0f, 20.0f, 20.0f };
	static const glm::vec3 pPos = { 0.0f, 5.0f, 0.0f };
	DeferredShadingScene::DeferredShadingScene():
		screenGShader("res/shaders/screen_quadG_vert.shader", "res/shaders/screen_quadG_frag.shader"),
		gShader("res/shaders/gBuffer_vert.shader", "res/shaders/gBuffer_frag.shader"),
		cube(MeshType::Cube),
		normalShader("res/shaders/point_light_vert.shader", "res/shaders/point_light_frag.shader")
	{
		// create fbo
		glGenFramebuffers(1, &Gfbo);
		glBindFramebuffer(GL_FRAMEBUFFER, Gfbo);
		
		// multiple render target
		glGenTextures(1, &pTex);
		glBindTexture(GL_TEXTURE_2D, pTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTex, 0);

		glGenTextures(1, &nTex);
		glBindTexture(GL_TEXTURE_2D, nTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, nTex, 0);

		// albedo is a color property, thus don't need that much precision nor memory
		glGenTextures(1, &aTex);
		glBindTexture(GL_TEXTURE_2D, aTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, aTex, 0);

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		// create depth buffer
		glGenRenderbuffers(1, &depthB);
		glBindRenderbuffer(GL_RENDERBUFFER, depthB);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthB);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer not valid" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		screenGShader.Bind();
		screenGShader.SetInt("posBuf", 0);
		screenGShader.SetInt("normalBuf", 1);
		screenGShader.SetInt("albedoBuf", 2);
		
		// screen vao and vbo
		glGenVertexArrays(1, &screenQuadVao);
		glBindVertexArray(screenQuadVao);

		std::vector<float> vertices = CreateScreenQuad();
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*) 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*) (2 * sizeof(float)));
		glBindVertexArray(0);
	}					  

	DeferredShadingScene::~DeferredShadingScene()
	{
		glDeleteFramebuffers(1, &Gfbo);
		glDeleteTextures(1, &pTex);
		glDeleteTextures(1, &nTex);
		glDeleteTextures(1, &aTex);
		glDeleteRenderbuffers(1, &depthB);
		glDeleteVertexArrays(1, &screenQuadVao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		gShader.Delete();
		screenGShader.Delete();
	}

	void DeferredShadingScene::OnUpdate(float deltaTime)
	{
		camera.processInput();
	}

	void DeferredShadingScene::OnRendering()
	{
		// Simple way of combining defer and forward rendering with blit func

		// if it is only pointlights, we could calculate the lowest light we could
		// accept, any other pixel that is far away would not be considered in the lighting calculation
		// however since gpu uses so-called simd and simt, almost always it would execute all branches of if, else if and else statements.
		// We could also just render a sphere that includes pixels that could be lit for each point light. When using this method, we need to call the front face of the sphere and render back face since the camera could potentially enter the sphere, and we also don't want to apply the light effects twice

		// Tile-based rendering is the most popular and cutting-edge rendering technique in the industry
		// which reduces space in register file as well as utilize gpu efficiently
		glBindFramebuffer(GL_FRAMEBUFFER, Gfbo);
		gl_renderer.Clear();
		gShader.Bind();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
		camera.SetViewProjectMat(gShader);
		

		for (int i = 0; i < 3; ++i) {
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
			gShader.SetMat4fv("model", model);
			cube.Draw(gShader);

		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		gl_renderer.Clear();
		glBindVertexArray(screenQuadVao);
		screenGShader.Bind();
		// set light pro
		screenGShader.SetVec3fv("light.dir", glm::vec3(1.0f, -1.0f, 1.0f));
		screenGShader.SetVec3fv("light.color", glm::vec3(1.0f, 1.0f, 1.0f));
		screenGShader.SetVec3fv("viewPos", camera.pos);
		// set up the necessary textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, nTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, aTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		//
		glBindFramebuffer(GL_READ_FRAMEBUFFER, Gfbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		normalShader.Bind();
		camera.SetViewProjectMat(normalShader);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		normalShader.SetMat4fv("model", model);
		normalShader.SetVec3fv("lights[0].color", pColor);
		normalShader.SetVec3fv("lights[0].pos", pPos);
		normalShader.SetVec3fv("viewPos", camera.pos);
		cube.Draw(normalShader);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		normalShader.SetMat4fv("model", model);
		cube.Draw(normalShader);
		
	}

	void DeferredShadingScene::OnImGuiRendering()
	{
		camera.OnImGuiRendering();
	}


}
