#include "PointShadowScene.h"
#include "Common.h"

namespace Scene {
	static void renderCube();
	static std::vector<glm::mat4> shadowMatrixes;
	static int shadowResW = 1024;
	static int shadowResH = 1024;
	static float near = 1.0f;
	static float far = 25.0f;
	static unsigned int depthCubemap;
	static unsigned int depthMapFBO;
	static glm::vec3 lightPos(0.0f, 5.0f, 0.0f);
PointShadowScene::PointShadowScene():
	m_shadow_map_shader("res/shaders/pointShadow_vert.shader", "res/shaders/pointShadow_frag.shader", "res/shaders/pointShadow_geom.shader"),
	m_phong("res/shaders/shadow_phong_vert.shader", "res/shaders/shadow_phong_frag.shader"),
	cube(MeshType::Cube),
	plane(MeshType::Quad)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	float aspect = (float)shadowResW / (float) shadowResH;
	//
	shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	//// generate 6 point light view matrix
	//// Note: since we are generating a cube map, we need to reverse the texture
	// since we put the point light at the center of the cube
	shadowMatrixes.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowMatrixes.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowMatrixes.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowMatrixes.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowMatrixes.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowMatrixes.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));



	
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_phong.SetInt("depthMap", 0);
}

PointShadowScene::~PointShadowScene() {
	m_shadow_map_shader.Delete();
	m_phong.Delete();
	glDeleteFramebuffers(1, &depthfbo);
	glDeleteTextures(1, &depthCubeMap);
}

void PointShadowScene::OnUpdate(float deltaTime) {
	camera.processInput();
}

void PointShadowScene::OnImGuiRendering() {
	camera.OnImGuiRendering();

}

void PointShadowScene::renderScene(m_Shader& shader) {
	
	// render plane
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
	shader.SetMat4fv("model", model);
	plane.RawDraw(shader);
	// render cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
	shader.SetMat4fv("model", model);
	cube.RawDraw(shader);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(-1.0f, -1.0f, 1.0f));
	shader.SetMat4fv("model", model);
	cube.RawDraw(shader);
}

void PointShadowScene::OnRendering() {
	// 0. create depth cubemap transformation matrices
	// -----------------------------------------------

	// 1. render shadow
	glViewport(0, 0, shadowResW, shadowResH);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_shadow_map_shader.Bind();
	// sent 6 point light perspective matrixes
	for (unsigned int i = 0; i < 6; ++i) {
		m_shadow_map_shader.SetMat4fv("shadowMatrices[" + std::to_string(i) + "]", shadowMatrixes[i]);
	}
	m_shadow_map_shader.SetVec3fv("lightPos", lightPos);
	m_shadow_map_shader.SetFloat("far_plane", far);
	renderScene(m_shadow_map_shader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. render the actual scene
	glViewport(0, 0, Width, Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_phong.Bind();
	glm::mat4 projection = camera.getProjection();
	glm::mat4 view = camera.getView();
	m_phong.SetMat4fv("projection", projection);
	m_phong.SetMat4fv("view", view);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	m_phong.SetFloat("far_plane", far);
	m_phong.SetVec3fv("lightPos", lightPos);
	m_phong.SetVec3fv("viewPos", camera.pos);
	renderScene(m_phong);
}
}