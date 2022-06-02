//
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#include <imgui/imgui.h>
//#include <imgui/imgui_impl_glfw.h>
//#include <imgui/imgui_impl_opengl3.h>
//
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <random>
//#include <vector>
//#include <unordered_map>
//#include "Renderer.h"
//#include "IndexBuffer.h"
//#include "VertexBuffer.h"
//#include "VertexArray.h"
//#include "VertexBufferLayout.h"
//#include "m_Shader.h"
//#include "Texture.h"
//#include "Common.h"
//
//// Scenes
//#include "Scene/Scene.h"
//#include "Scene/TextureScene.h"
//#include "Scene/TransformScene.h"
//#include "Scene/CoordinateScene.h"
//#include "Scene/CameraScene.h"
//#include "Scene/LightScene.h"
//#include "Scene/LightTypeScene.h"
//#include "Scene/SimpleModelScene.h"
//#include "Scene/DepthScene.h"
//#include "Scene/StencilScene.h"
//#include "Scene/BlendScene.h"
//#include "Scene/FramebufferScene.h"
//#include "Scene/CubeMapScene.h"
//#include "Scene/GeometryScene.h"
//#include "Scene/GeometryIIScene.h"
//#include "Scene/InstanceScene.h"
//#include "Scene/InstanceIIScene.h"
//#include "Scene/MSAAScene.h"
//#include "Scene/ShadowScene.h"
//#include "Scene/PointShadowScene.h"
//#include "Scene/NormalMapScene.h"
//#include "Scene/HdrBloomScene.h"
//#include "Scene/DeferredShadingScene.h"
//
//
//
//static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//
//unsigned int loadTexture(const char* path, bool gammaCorrection);
//void renderQuad();
//void renderCube();
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//int Width = 800;
//int Height = 600;
//// camera
//CameraFps camera;
////float lastX = (float)SCR_WIDTH / 2.0;
////float lastY = (float)SCR_HEIGHT / 2.0;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//float DeltaTime = 0.0f;
//Renderer gl_renderer = Renderer();
//GLFWwindow* Window = nullptr;
//float lerp(float a, float b, float f)
//{
//	return a + f * (b - a);
//}
//
//int main()
//{
//	// glfw: initialize and configure
//	// ------------------------------
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//	// glfw window creation
//	// --------------------
//	Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//	if (Window == NULL)
//	{
//		std::cout << "Failed to create GLFW Window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(Window);
//	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
//
//	// tell GLFW to capture our mouse
//	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//	// glad: load all OpenGL function pointers
//	// ---------------------------------------
//	if (GLEW_OK != glewInit())
//		std::cout << "GlewInit Error!" << std::endl;
//	// configure global opengl state
//	// -----------------------------
//	glEnable(GL_DEPTH_TEST);
//
//	// build and compile shaders
//	// -------------------------
//	m_Shader shaderGeometryPass("res/shaders/gBuffer_vert.shader", "res/shaders/gBuffer_frag.shader");
//	m_Shader shaderLightingPass("res/shaders/screen_quadG_vert.shader", "res/shaders/screen_quadG_frag.shader");
//	m_Shader shaderSSAO("res/shaders/ssao_vert.shader", "res/shaders/ssao_frag.shader");
//	m_Shader shaderSSAOBlur("src/9.ssao.vs", "src/9.ssao_blur.fs");
//
//	// load models
//	// -----------
//	Model backpack("res/Model/simple_model/backpack.mobj");
//
//	// configure g-buffer framebuffer
//	// ------------------------------
//	unsigned int gBuffer;
//	glGenFramebuffers(1, &gBuffer);
//	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
//	unsigned int gPosition, gNormal, gAlbedo;
//	// position color buffer
//	glGenTextures(1, &gPosition);
//	glBindTexture(GL_TEXTURE_2D, gPosition);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
//	// normal color buffer
//	glGenTextures(1, &gNormal);
//	glBindTexture(GL_TEXTURE_2D, gNormal);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
//	// color + specular color buffer
//	glGenTextures(1, &gAlbedo);
//	glBindTexture(GL_TEXTURE_2D, gAlbedo);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
//	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
//	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
//	glDrawBuffers(3, attachments);
//	// create and attach depth buffer (renderbuffer)
//	unsigned int rboDepth;
//	glGenRenderbuffers(1, &rboDepth);
//	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
//	// finally check if framebuffer is complete
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "Framebuffer not complete!" << std::endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	// also create framebuffer to hold SSAO processing stage 
//	// -----------------------------------------------------
//	unsigned int ssaoFBO, ssaoBlurFBO;
//	glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
//	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
//	unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
//	// SSAO color buffer
//	glGenTextures(1, &ssaoColorBuffer);
//	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "SSAO Framebuffer not complete!" << std::endl;
//	// and blur stage
//	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
//	glGenTextures(1, &ssaoColorBufferBlur);
//	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	// generate sample kernel
//	// ----------------------
//	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
//	std::default_random_engine generator;
//	std::vector<glm::vec3> ssaoKernel;
//	for (unsigned int i = 0; i < 64; ++i)
//	{
//		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
//		sample = glm::normalize(sample);
//		sample *= randomFloats(generator);
//		float scale = float(i) / 64.0f;
//
//		// scale samples s.t. they're more aligned to center of kernel
//		scale = lerp(0.1f, 1.0f, scale * scale);
//		sample *= scale;
//		ssaoKernel.push_back(sample);
//	}
//
//	// generate noise texture
//	// ----------------------
//	std::vector<glm::vec3> ssaoNoise;
//	for (unsigned int i = 0; i < 16; i++)
//	{
//		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
//		ssaoNoise.push_back(noise);
//	}
//	unsigned int noiseTexture; glGenTextures(1, &noiseTexture);
//	glBindTexture(GL_TEXTURE_2D, noiseTexture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	// lighting info
//	// -------------
//	glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
//	glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);
//
//	// shader configuration
//	// --------------------
//	shaderLightingPass.Bind();
//	shaderLightingPass.SetInt("gPosition", 0);
//	shaderLightingPass.SetInt("gNormal", 1);
//	shaderLightingPass.SetInt("gAlbedo", 2);
//	shaderLightingPass.SetInt("ssao", 3);
//	shaderSSAO.Bind();
//	shaderSSAO.SetInt("gPosition", 0);
//	shaderSSAO.SetInt("gNormal", 1);
//	shaderSSAO.SetInt("texNoise", 2);
//	shaderSSAOBlur.Bind();
//	shaderSSAOBlur.SetInt("ssaoInput", 0);
//
//	// render loop
//	// -----------
//	while (!glfwWindowShouldClose(Window))
//	{
//		
//		if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//			glfwSetWindowShouldClose(Window, true);
//		// per-frame time logic
//		// --------------------
//		float currentFrame = static_cast<float>(glfwGetTime());
//		DeltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//		
//		
//		// render
//		// ------
//		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		// 1. geometry pass: render scene's geometry/color data into gbuffer
//		// -----------------------------------------------------------------
//		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glm::mat4 projection = camera.getProjection();
//		glm::mat4 view = camera.getView();
//		glm::mat4 model = glm::mat4(1.0f);
//		shaderGeometryPass.Bind();
//		shaderGeometryPass.SetMat4fv("projection", projection);
//		shaderGeometryPass.SetMat4fv("view", view);
//
//		// backpack model on the floor
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
//		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
//		model = glm::scale(model, glm::vec3(1.0f));
//		shaderGeometryPass.SetMat4fv("model", model);
//		backpack.Draw(shaderGeometryPass);
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//
//		// 2. generate SSAO texture
//		// ------------------------
//		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
//		glClear(GL_COLOR_BUFFER_BIT);
//		shaderSSAO.Bind();
//		// Send kernel + rotation 
//		for (unsigned int i = 0; i < 64; ++i)
//			shaderSSAO.SetVec3fv("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
//		shaderSSAO.SetMat4fv("projection", projection);
//		shaderSSAO.SetMat4fv("view", camera.getView());
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, gPosition);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, gNormal);
//		glActiveTexture(GL_TEXTURE2);
//		glBindTexture(GL_TEXTURE_2D, noiseTexture);
//		renderQuad();
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//
//		// 3. blur SSAO texture to remove noise
//		// ------------------------------------
//		/*glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
//		glClear(GL_COLOR_BUFFER_BIT);
//		shaderSSAOBlur.Bind();
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
//		renderQuad();
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
//
//
//		// 4. lighting pass: traditional deferred Blinn-Phong lighting with added screen-space ambient occlusion
//		// -----------------------------------------------------------------------------------------------------
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		shaderLightingPass.Bind();
//		// send light relevant uniforms
//		glm::vec3 lightPosView = glm::vec3(camera.getView() * glm::vec4(lightPos, 1.0));
//		shaderLightingPass.SetVec3fv("light.Position", lightPosView);
//		shaderLightingPass.SetVec3fv("light.Color", lightColor);
//		// Update attenuation parameters
//		const float linear = 0.09f;
//		const float quadratic = 0.032f;
//		shaderLightingPass.SetFloat("light.Linear", linear);
//		shaderLightingPass.SetFloat("light.Quadratic", quadratic);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, gPosition);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, gNormal);
//		glActiveTexture(GL_TEXTURE2);
//		glBindTexture(GL_TEXTURE_2D, gAlbedo);
//		glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
//		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
//		renderQuad();
//
//
//		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//		// -------------------------------------------------------------------------------
//		glfwSwapBuffers(Window);
//		glfwPollEvents();
//		camera.processInput();
//	}
//
//	glfwTerminate();
//	return 0;
//}
//
//// renderCube() renders a 1x1 3D cube in NDC.
//// -------------------------------------------------
//unsigned int cubeVAO = 0;
//unsigned int cubeVBO = 0;
//void renderCube()
//{
//	// initialize (if necessary)
//	if (cubeVAO == 0)
//	{
//		float vertices[] = {
//			// back face
//			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
//			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
//			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
//			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
//			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
//			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
//			// front face
//			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
//			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
//			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
//			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
//			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
//			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
//			// left face
//			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
//			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
//			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
//			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
//			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
//			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
//			// right face
//			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
//			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
//			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
//			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
//			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
//			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
//			// bottom face
//			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
//			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
//			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
//			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
//			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
//			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
//			// top face
//			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
//			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
//			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
//			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
//			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
//			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
//		};
//		glGenVertexArrays(1, &cubeVAO);
//		glGenBuffers(1, &cubeVBO);
//		// fill buffer
//		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//		// link vertex attributes
//		glBindVertexArray(cubeVAO);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindVertexArray(0);
//	}
//	// render Cube
//	glBindVertexArray(cubeVAO);
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//	glBindVertexArray(0);
//}
//
//
//// renderQuad() renders a 1x1 XY quad in NDC
//// -----------------------------------------
//unsigned int quadVAO = 0;
//unsigned int quadVBO;
//void renderQuad()
//{
//	if (quadVAO == 0)
//	{
//		float quadVertices[] = {
//			// positions        // texture Coords
//			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
//			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
//			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//		};
//		// setup plane VAO
//		glGenVertexArrays(1, &quadVAO);
//		glGenBuffers(1, &quadVBO);
//		glBindVertexArray(quadVAO);
//		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	}
//	glBindVertexArray(quadVAO);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	glBindVertexArray(0);
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and 
//	// height will be significantly larger than specified on retina displays.
//	glViewport(0, 0, width, height);
//}