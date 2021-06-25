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
	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;
	float width;
	float height;
	float near;
	float far;
	void processInput();
	void SetViewProjectMat(m_Shader* const m_shader);
private:
	glm::mat4 view;
	glm::mat4 projection;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float sensitivity;
	float cameraSpeed;
	bool firstTime;
	void CameraCallBackInit();
};