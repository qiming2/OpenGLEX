#include <random>
#include "DeferredShadingScene.h"

static float lerp(float a, float b, float f) {
	return a * (1.0f - f) + b * f;
}
static unsigned int Gfbo; // gbuffer fbo
static unsigned int pTex; // position Texture
static unsigned int nTex; // normal Texture
static unsigned int aTex; // albedo Texture
static unsigned int depthB;
// unsigned int sTex; specular intensity, don't need this
// since we can just store it in albedo texture
static unsigned int screenQuadVao;
static unsigned int vbo;
static unsigned int ibo;
static unsigned int ssaoFBO;
static unsigned int ssaoColorBuffer;
static unsigned int noiseTexture;
static bool useSSAO = true;
namespace Scene {
	static const glm::vec3 pColor = { 20.0f, 20.0f, 20.0f };
	static const glm::vec3 pPos = { 0.0f, 5.0f, 0.0f };
	DeferredShadingScene::DeferredShadingScene() :
		screenGShader("res/shaders/screen_quadG_vert.shader", "res/shaders/screen_quadG_frag.shader"),
		gShader("res/shaders/gBuffer_vert.shader", "res/shaders/gBuffer_frag.shader"),
		cube(MeshType::Cube),
		normalShader("res/shaders/point_light_vert.shader", "res/shaders/point_light_frag.shader"),
		backpack("res/Model/simple_model/backpack.mobj", false),
		ssaoShader("res/shaders/ssao_vert.shader", "res/shaders/ssao_frag.shader")
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
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

		// Using SSAO to generate ssao buffer
		glGenFramebuffers(1, &ssaoFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

		glGenTextures(1, &ssaoColorBuffer);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Width, Height, 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "SSAO Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Generate Kernel for sampling occulusion factors
		std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f);
		std::default_random_engine generator;
		float scale;
		for (size_t i = 0; i < 64; ++i) {
			glm::vec3 sample(
				randomFloats(generator) * 2.0f - 1.0f, // normalize to [-1.0, 1.0]
				randomFloats(generator) * 2.0f - 1.0f,
				randomFloats(generator)
			);
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			scale = (float)i / 64.0f;
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		// Random Kernel Rotation

		for (size_t i = 0; i < 16; ++i) {
			glm::vec3 noise(
				randomFloats(generator) * 2.0f - 1.0f,
				randomFloats(generator) * 2.0f - 1.0f,
				0.0f
			);
			ssaoNoise.push_back(noise);
		}

		////////////////////////// ??????? wth is this
		// duplicate names with the file global variable
		// this variable would be initialized but the global one
		//unsigned int noiseTexture;
		glGenTextures(1, &noiseTexture);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		screenGShader.Bind();
		screenGShader.SetInt("gPosition", 0);
		screenGShader.SetInt("gNormal", 1);
		screenGShader.SetInt("gAlbedo", 2);
		screenGShader.SetInt("ssao", 3);



		ssaoShader.Bind();
		ssaoShader.SetInt("gPosition", 0);
		ssaoShader.SetInt("gNormal", 1);
		ssaoShader.SetInt("texNoise", 2);


		// screen vao and vbo
		glGenVertexArrays(1, &screenQuadVao);
		glBindVertexArray(screenQuadVao);

		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &screenQuadVao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(screenQuadVao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		/*std::vector<float> vertices = CreateScreenQuad();
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*) 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*) (2 * sizeof(float)));
		glBindVertexArray(0);*/

		// SSAO PART ----------------------------------------

		
		
		
		
	}					  

	DeferredShadingScene::~DeferredShadingScene()
	{
		glDeleteFramebuffers(1, &Gfbo);
		glDeleteTextures(1, &pTex);
		glDeleteTextures(1, &nTex);
		glDeleteTextures(1, &aTex);
		glDeleteTextures(1, &noiseTexture);
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

		// if it is only pointlights, we could calculate the nearest light we could
		// accept, any other pixel that is far away would not be considered in the lighting calculation
		// however since gpu uses so-called simd and simt, almost always it would execute all branches of if, else if and else statements.
		// We could also just render a sphere that includes pixels that could be lit for each point light. When using this method, we need to call the front face of the sphere and render back face since the camera could potentially enter the sphere, and we also don't want to apply the light effects twice

		// Tile-based rendering is the most popular and cutting-edge rendering technique in the industry
		// which reduces space in register file as well as utilize gpu efficiently
		glEnable(GL_DEPTH_TEST);
		gl_renderer.Clear();

		////////////////// GEOMETRY PASS ///////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, Gfbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gShader.Bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		gShader.SetMat4fv("model", model);
		camera.SetViewProjectMat(gShader);
		backpack.Draw(gShader);
		gShader.Unbind();

		/////////////////////// SSAO PASS /////////////////////
		if (useSSAO) {
			glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
			glClear(GL_COLOR_BUFFER_BIT);
			ssaoShader.Bind();
			for (unsigned int i = 0; i < 64; ++i)
				ssaoShader.SetVec3fv("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
			//glUniform3fv(glGetUniformLocation(ssaoShader.getID(), "samples"), 64, &ssaoKernel[0][0]);
			ssaoShader.SetMat4fv("view", camera.getView());
			ssaoShader.SetMat4fv("projection", camera.getProjection());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pTex);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, nTex);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, noiseTexture);
			glBindVertexArray(screenQuadVao);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}
		

		// SSAO PASS END
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
		glActiveTexture(GL_TEXTURE3);
		if (useSSAO) {
			screenGShader.SetInt("useSSAO", 1);
			glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		}
		else {
			screenGShader.SetInt("useSSAO", 0);
		}
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		
		



		//
		/*glBindFramebuffer(GL_READ_FRAMEBUFFER, Gfbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
		/*normalShader.Bind();
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
		cube.Draw(normalShader);*/
		
	}

	void DeferredShadingScene::OnImGuiRendering()
	{
		ImGui::Checkbox("SSAO", &useSSAO);
		camera.OnImGuiRendering();
	}


}
