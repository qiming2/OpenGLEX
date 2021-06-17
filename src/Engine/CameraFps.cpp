#include "CameraFps.h"

CameraFps::CameraFps():
	pos(glm::vec3(0.0f, 0.0f, 0.0f)),
	target(glm::vec3(0.0f, 0.0f, -1.0f)),
	up(glm::vec3(0.0f, 1.0f, 0.0f))
{

}

CameraFps::~CameraFps()
{
}


CameraFps::CameraFps(glm::vec3 pos, glm::vec3 target, glm::vec3 up):
	pos(pos),
	target(target),
	up(up)
{

}

glm::mat4 CameraFps::getView() {
	glm::mat4 ret(1.0f);

	glm::vec3 zaxis = glm::normalize(pos - target);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), -pos);

	glm::mat4 rotation = glm::mat4(1.0f);

	rotation[0][0] = xaxis[0];
	rotation[1][0] = xaxis[1];
	rotation[2][0] = xaxis[2];
	rotation[0][1] = yaxis[0];
	rotation[1][1] = yaxis[1];
	rotation[2][1] = yaxis[2];
	rotation[0][2] = zaxis[0];
	rotation[1][2] = zaxis[1];
	rotation[2][2] = zaxis[2];

	return rotation * translation;
}
