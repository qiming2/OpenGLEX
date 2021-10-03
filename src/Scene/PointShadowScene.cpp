#include "PointShadowScene.h"
#include "Common.h"

namespace Scene {
	static std::vector<glm::mat4> views;
	static int shadowResW = 1024;
	static int shadowResH = 1024;
PointShadowScene::PointShadowScene():
	cube(MeshType::Cube),
	cube2(MeshType::Cube),
	plane(MeshType::Quad)
{
	planeModel = glm::mat4(1.0f);
	planeModel = glm::translate(planeModel, glm::vec3(0.0, -3.0f, 0.0f));
	planeModel = glm::scale(planeModel, glm::vec3(10.0f, 0.0f, 10.0f));
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	model2 = glm::mat4(1.0f);
	model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f));
	model2 = glm::rotate(model2, glm::radians(45.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
	// generate point light depth buffer
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	// we need to generate six faces of point light since
	for (int i = 0; i < 6; ++i) {
		// Generate 6 textures
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowResW, shadowResH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// generate framebuffer
	glGenFramebuffers(1, &depthfbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depthfbo);
	// bind cube map texture to this fbo
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, depthCubeMap, 0);
	// since we don't have a depth buffer, we need to tell opengl that
	// we don't need to render the scene only need the depth component
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// light configuration
	light.pos = glm::vec3(0.0f, 5.0f, 0.0f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light.ambient= glm::vec3(0.1f, 0.1f, 0.1f);
	float aspect = (float)shadowResW / shadowResH;
	near = 0.1f;
	far = 25.0f;
	shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	// generate 6 point light view matrix
	views.push_back(shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	views.push_back(shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	views.push_back(shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	views.push_back(shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	views.push_back(shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	views.push_back(shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));


}

PointShadowScene::~PointShadowScene() {

}

void PointShadowScene::OnUpdate(float deltaTime) {
	camera.processInput();
}

void PointShadowScene::OnImGuiRendering() {
	camera.OnImGuiRendering();

}

void PointShadowScene::OnRendering() {
	// we need two render passes

	// 1. render shadow
	glViewport(0, 0, shadowResW, shadowResH);
	glBindFramebuffer(GL_FRAMEBUFFER, depthfbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	// 2. render the actual scene
	
}

}