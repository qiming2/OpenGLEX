#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <memory>
#include "Common.h"
#include "m_Shader.h"

class CameraFps {
public:
	CameraFps();
	CameraFps(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	~CameraFps();
	glm::mat4 getView();
	const glm::mat4& getPojection();
	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;
	float width;
	float height;
	float near;
	float far;
	float sensitivity;
	float cameraSpeed;
	void processInput();
	void SetViewProjectMat(m_Shader* const m_shader);
	void SetViewProjectMat(m_Shader& m_shader);
	void OnImGuiRendering();
private:
	glm::mat4 view;
	glm::mat4 projection;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	bool firstTime;
	void CameraCallBackInit();
};