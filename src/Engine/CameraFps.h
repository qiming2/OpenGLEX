#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Common.h"

class CameraFps {
public:
	CameraFps();
	CameraFps(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	~CameraFps();
	glm::mat4 getView();
	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;
};