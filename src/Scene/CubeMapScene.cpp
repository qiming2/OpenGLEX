#include "CubeMapScene.h"
#include "Common.h"
#include "Texture.h"

namespace Scene {
	static std::vector<std::string> concatFacePath(const std::string path) {
		Texture::SetFlip(false);
		return {path + "right.jpg", path + "left.jpg", path + "top.jpg", path + "bottom.jpg", path + "front.jpg", path + "back.jpg"};
	}

	CubeMapScene::CubeMapScene():
		cube(MeshType::Cube),
		skybox(MeshType::Cube),
		cm(concatFacePath("res/CubeMap/skybox_ex/"), GL_TEXTURE0),
		normalShader("res/shaders/envmap_vert.shader", "res/shaders/envmap_frag.shader"),
		skyboxShader("res/shaders/cube_map_vert.shader", "res/shaders/cube_map_frag.shader"),
		model(1.0f)
	{
		scale = 1.0f;
		skyboxShader.Bind();
		skyboxShader.SetInt("cubeMap", 0);
		normalShader.Bind();
		normalShader.SetInt("skybox", 0);
	}

	CubeMapScene::~CubeMapScene() {
		normalShader.Delete();
		skyboxShader.Delete();
		cube.Delete();
		skybox.Delete();
		cm.Delete();
	}

	void CubeMapScene::OnUpdate(float deltaTime) {
		camera.processInput();
	}

	void CubeMapScene::OnImGuiRendering() {
		camera.OnImGuiRendering();
		// Check whether scaling will affect skybox rendering
		ImGui::SliderFloat("Scale", &scale, 0.0f, 10000.f, 0, 0);
	}

	void CubeMapScene::OnRendering() {
		gl_renderer.Clear();
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		
		normalShader.Bind();
		glActiveTexture(GL_TEXTURE0);
		cm.Bind();
		camera.SetViewProjectMat(normalShader);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		normalShader.SetMat4fv("model", model);
		normalShader.SetVec3fv("viewPos", camera.pos);
		cube.Draw(normalShader);
		
		// We don't want to write depth buffer
		// for skybox, since it would always be the
		// background

		// since we are setting skybox vertexes as always being rendered last
		// which we set pos = xyww, after perspective division
		// z component will 1.0
		// we need set less and equal to 1.0 (could be other values maybe?)
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		skyboxShader.Bind();
		glActiveTexture(GL_TEXTURE0);
		skyboxShader.SetMat4fv("projection", camera.getPojection());
		//skyboxShader.SetMat4fv("view", glm::mat4(glm::mat3(camera.getView())));
		// This is the key of the cube map, since we don't move the cube at all
		// or in other words, we move the cube with the camera
		skyboxShader.SetMat4fv("view", glm::mat4(glm::mat3(camera.getView())));

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		skyboxShader.SetMat4fv("model", model);
		cm.Bind();
		skybox.Draw(skyboxShader);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
}