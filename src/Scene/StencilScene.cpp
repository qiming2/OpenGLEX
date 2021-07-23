#include "StencilScene.h"
#include "GeometryUtil.h"

namespace Scene {


	// Can learn more about z-fighting
	StencilScene::StencilScene():
		model_m(1.0f),
		model("res/Model/simple_model/backpack.mobj", false),
		shader("res/shaders/simple_model_vert.shader", "res/shaders/simple_model_frag.shader"),
		outlineShader("res/shaders/outline_vert.shader", "res/shaders/outline_frag.shader")
	{

	}

	StencilScene::~StencilScene() {
		model.Delete();
		shader.Delete();
		outlineShader.Delete();
	}

	void StencilScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}

	void StencilScene::OnImGuiRendering() {
		ImGui::SliderFloat("camera speed", &camera.cameraSpeed, 0.0f, 1000.0f);
		ImGui::ColorEdit3("Outline Color 1", (float*)&color1);
		ImGui::ColorEdit3("Outline Color 2", (float*)&color2);
	}

	void StencilScene::OnRendering() {
		// Draw outlines, also make outlines always appear before everything
		gl_renderer.Clear();

		OutlineStartSetting();
		model_m = glm::mat4(1.0f);
		DrawSingleObject(model_m, shader);

		OutlineDrawSetting();
		outlineShader.Bind();
		outlineShader.SetVec3fv("outline_color", color1.x, color1.y, color1.z);
		DrawSingleOutline(model_m, outlineShader);
		OutlineEndSetting();

		OutlineStartSetting();
		model_m = glm::mat4(1.0f);
		model_m = glm::translate(model_m, glm::vec3(0.0f, 0.0f, 10.0f));
		DrawSingleObject(model_m, shader);

		OutlineDrawSetting();
		outlineShader.Bind();
		outlineShader.SetVec3fv("outline_color", color2.x, color2.y, color2.z);
		DrawSingleOutline(model_m, outlineShader);
		OutlineEndSetting();
		
	}

	void StencilScene::DrawObjects(m_Shader& shader) {
		shader.Bind();
		camera.SetViewProjectMat(&shader);

		model_m = glm::mat4(1.0f);
		shader.SetMat4fv("model", model_m);
		model.Draw(shader);

		model_2 = glm::mat4(1.0f);
		model_2 = glm::translate(model_2, glm::vec3(0.0f, 0.0f, -5.0f));
		shader.SetMat4fv("model", model_2);
		model.Draw(shader);
	}

	void StencilScene::DrawOutline(m_Shader& shader) {
		shader.Bind();
		camera.SetViewProjectMat(&shader);
		glm::vec3 scaleF = glm::vec3(1.05f, 1.05f, 1.05f);
		model_m = glm::scale(model_m, scaleF);
		shader.SetMat4fv("model", model_m);
		model.Draw(shader);

		model_2 = glm::scale(model_2, scaleF);
		shader.SetMat4fv("model", model_2);
		model.Draw(shader);
	}

	void StencilScene::DrawSingleObject(glm::mat4& model_temp, m_Shader& shader) {
		shader.Bind();
		camera.SetViewProjectMat(&shader);

		shader.SetMat4fv("model", model_temp);
		model.Draw(shader);
	}

	void StencilScene::DrawSingleOutline(glm::mat4& model_temp, m_Shader& shader) {
		shader.Bind();
		camera.SetViewProjectMat(&shader);
		glm::vec3 scaleF = glm::vec3(1.05f, 1.05f, 1.05f);
		model_temp = glm::scale(model_temp, scaleF);
		shader.SetMat4fv("model", model_temp);
		model.Draw(shader);
	}

	void StencilScene::OutlineStartSetting() {
		// reset everthing at the start
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		// keep the current stencil value when either of stencil or depth test does not pass
		// replace the value with the reference value we specified
		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
		glDepthFunc(GL_LESS);
		
		// Always pass the stencil test when drawing actually objects
		// to set stencil values for the body
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF); // enable writing, value about to be written to the buffer would be ANDed with this value
	}

	void StencilScene::OutlineDrawSetting() {
		// Now disable write to stencil and disable depth test
		glStencilMask(0x00);
		// Draw a scaled up version of all objects, check whether
		// the stencil value is not set to 1
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	}

	void StencilScene::OutlineEndSetting() {
		// always reset everthing in case we want to draw more things later
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);
	}
}