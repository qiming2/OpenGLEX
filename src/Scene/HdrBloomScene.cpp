#include "HdrBloomScene.h"


namespace Scene {
	float exposure = 0.05f;
	const glm::vec3 color1 = glm::vec3(100.0f, 100.0f, 100.0f);
	const glm::vec3 pos1 = glm::vec3(0.0f, 0.0f, -9.0f);
	const glm::vec3 color2 = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 pos2 = glm::vec3(0.0f, 0.8f, -1.0f);
	HdrBloomScene::HdrBloomScene() :
		shader("res/shaders/hdr_vert.shader", "res/shaders/hdr_frag.shader"),
		light_shader("res/shaders/point_light_vert.shader", "res/shaders/point_light_frag.shader"),
		cube(MeshType::Cube)
{
		light_shader.Bind();
		light_shader.SetVec3fv("lights[0].color", color1);
		light_shader.SetVec3fv("lights[0].pos", pos1);
		light_shader.SetVec3fv("lights[1].color", color2);
		light_shader.SetVec3fv("lights[1].pos", pos2);

		// render to a hdr buffer without clamping out_color to 0.0f and 1.0f
		glGenFramebuffers(1, &hdrfbo);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrfbo);
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// create depth buffer
		glGenRenderbuffers(1, &depthrbo);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);

		// attach color and depth buffer to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrbo);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		std::vector<float> vertices = CreateScreenQuad();
		glGenVertexArrays(1, &screen_vao);
		glBindVertexArray(screen_vao);
		glGenBuffers(1, &screen_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 4, (void*)(sizeof(float) * 2));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		shader.Bind();
		shader.SetInt("texture_hdr", 0);
}

HdrBloomScene::~HdrBloomScene() {
	glDeleteVertexArrays(1, &screen_vao);
	glDeleteBuffers(1, &screen_vbo);
	
	glDeleteTextures(1, &colorBuffer);
	glDeleteRenderbuffers(1, &depthrbo);
	glDeleteFramebuffers(1, &hdrfbo);
	shader.Delete();
	light_shader.Delete();
}

void HdrBloomScene::OnUpdate(float deltaTime) {
	camera.processInput();
}

void HdrBloomScene::OnImGuiRendering() {
	camera.OnImGuiRendering();
	ImGui::SliderFloat("Exposure", &exposure, 0.05f, 10.0f);
}

void HdrBloomScene::OnRendering()  {
	glBindFramebuffer(GL_FRAMEBUFFER, hdrfbo);
	gl_renderer.Clear();
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 20.0f));
	light_shader.Bind();
	camera.SetViewProjectMat(light_shader);
	light_shader.SetInt("inverse", true);
	light_shader.SetMat4fv("model", model);
	cube.Draw(light_shader);
	
	// second pass, do tone mapping
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	gl_renderer.Clear();
	glBindVertexArray(screen_vao);
	shader.Bind();
	shader.SetInt("hdr", true);
	shader.SetFloat("exposure", exposure);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

}