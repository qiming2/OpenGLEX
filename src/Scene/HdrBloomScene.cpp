#include "HdrBloomScene.h"


// Bloom with hdr and two-pass separate gaussian blur
namespace Scene {
	const int blur_times = 5;
	float exposure = 0.05f;
	const glm::vec3 color1 = glm::vec3(50.0f, 50.0f, 50.0f);
	const glm::vec3 pos1 = glm::vec3(3.0f, 3.0f, 0.0f);
	const glm::vec3 color2 = glm::vec3(20.0f, 20.0f, 0.0f);
	const glm::vec3 pos2 = glm::vec3(0.0f, 1.0f, -1.0f);
	const glm::vec3 color3 = glm::vec3(0.0f, 30.0f, 20.0f);
	const glm::vec3 pos3 = glm::vec3(-3.0f, 4.0f, -3.0f);
	HdrBloomScene::HdrBloomScene() :
		shader("res/shaders/hdr_vert.shader", "res/shaders/hdr_frag.shader"),
		light_shader("res/shaders/point_light_vert.shader", "res/shaders/point_light_frag.shader"),
		cube(MeshType::Cube),
		quad(MeshType::Quad),
		light_cube("res/shaders/light_cube_vert.shader", "res/shaders/light_cube_frag.shader"),
		blur_pass("res/shaders/blur_pass_vert.shader", "res/shaders/blur_pass_frag.shader")
{
		light_shader.Bind();
		light_shader.SetVec3fv("lights[0].color", color1);
		light_shader.SetVec3fv("lights[0].pos", pos1);
		light_shader.SetVec3fv("lights[1].color", color2);
		light_shader.SetVec3fv("lights[1].pos", pos2);
		light_shader.SetVec3fv("lights[1].color", color3);
		light_shader.SetVec3fv("lights[1].pos", pos3);

		// render to a hdr buffer without clamping out_color to 0.0f and 1.0f
		glGenFramebuffers(1, &hdrfbo);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrfbo);
		glGenTextures(2, colorBuffer);
		for (size_t i = 0; i < 2; ++i) {
			glBindTexture(GL_TEXTURE_2D, colorBuffer[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			// attach color and depth buffer to framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffer[i], 0);			
		}

		// create depth buffer
		glGenRenderbuffers(1, &depthrbo);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrbo);

		// Since we are writing to two color buffers, we need to specify it
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Screen Quad
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
		shader.SetInt("texture_brightness", 1);
		shader.Unbind();

		// ping pong color buffer for doing 
		glGenFramebuffers(2, blurfbo);
		glGenTextures(2, blurBuf);
		for (unsigned int i = 0; i < 2; ++i) {
			glBindFramebuffer(GL_FRAMEBUFFER, blurfbo[i]);
			glBindTexture(GL_TEXTURE_2D, blurBuf[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurBuf[i], 0);
		}
		blur_pass.Bind();
		blur_pass.SetInt("texture_hdr1", 0);
}

HdrBloomScene::~HdrBloomScene() {
	glDeleteVertexArrays(1, &screen_vao);
	glDeleteBuffers(1, &screen_vbo);
	
	glDeleteTextures(2, colorBuffer);
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
	// There are some redundancy and definitely can be
	// factorized.
	// First pass render scene and light cube
	glBindFramebuffer(GL_FRAMEBUFFER, hdrfbo);
	gl_renderer.Clear();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 3.0f, -1.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	light_shader.Bind();
	camera.SetViewProjectMat(light_shader);
	light_shader.SetMat4fv("model", model);
	cube.Draw(light_shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 2.0f, 0.0));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(-1.0f, 1.0f, -1.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	light_shader.SetMat4fv("model", model);
	cube.Draw(light_shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
	light_shader.SetMat4fv("model", model);
	quad.Draw(light_shader);

	// Light cubes
	light_cube.Bind();
	camera.SetViewProjectMat(light_cube);
	model = glm::translate(glm::mat4(1.0f), pos1);
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	light_cube.SetMat4fv("model", model);
	light_cube.SetVec3fv("lightColor", color1);
	cube.RawDraw(light_cube);

	model = glm::translate(glm::mat4(1.0f), pos2);
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	light_cube.SetMat4fv("model", model);
	light_cube.SetVec3fv("lightColor", color2);
	cube.RawDraw(light_cube);

	model = glm::translate(glm::mat4(1.0f), pos3);
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	light_cube.SetMat4fv("model", model);
	light_cube.SetVec3fv("lightColor", color3);
	cube.RawDraw(light_cube);

	// blur pass, we can do separate 1-D gaussian blur in both x and y axises
	// this would yield the same result as if we do a 2-D gaussian blur
	blur_pass.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer[1]);
	glBindVertexArray(screen_vao);
	glBindFramebuffer(GL_FRAMEBUFFER, blurfbo[0]);
	gl_renderer.Clear();
	glBindFramebuffer(GL_FRAMEBUFFER, blurfbo[1]);
	gl_renderer.Clear();
	for (unsigned int i = 0; i < blur_times; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, blurfbo[0]);
		blur_pass.SetInt("horizontal", 1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, blurBuf[0]);

		glBindFramebuffer(GL_FRAMEBUFFER, blurfbo[1]);
		blur_pass.SetInt("horizontal", 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, blurBuf[1]);
	}

	// second pass, do tone mapping
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	gl_renderer.Clear();
	glBindVertexArray(screen_vao);
	shader.Bind();
	shader.SetInt("hdr", true);
	shader.SetFloat("exposure", exposure);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer[0]);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, blurBuf[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

}