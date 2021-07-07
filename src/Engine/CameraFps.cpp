#include "Common.h"
#include "GLFW/glfw3.h"
#include "CameraFps.h"



static float xPos = 0.0f;
static float yPos = 0.0f;
static float fov = 45.0f;
static void mouse_callback(GLFWwindow* Window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* Window, double xoffset, double yoffset);

CameraFps::CameraFps():
	pos(glm::vec3(0.0f, 0.0f, 0.0f)),
	target(glm::vec3(0.0f, 0.0f, -1.0f)),
	up(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw(-90.0f),
	pitch(0.0f),
	lastX(0.0f),
	lastY(0.0f),
	sensitivity(2.0f),
	cameraSpeed(5.0f),
	width(Width),
	height(Height),
	near(0.1f),
	far(100.0f),
	firstTime(true)
{
	CameraCallBackInit();
}

CameraFps::~CameraFps()
{
}


CameraFps::CameraFps(glm::vec3 pos, glm::vec3 target, glm::vec3 up):
	pos(pos),
	target(target),
	up(up),
	yaw(-90.0f),
	pitch(0.0f),
	lastX(0.0f),
	lastY(0.0f),
	sensitivity(0.2f),
	cameraSpeed(10.0f),
	width(Width),
	height(Height),
	near(0.1f),
	far(100.0f)
{
	CameraCallBackInit();
}

// View matrix practice
glm::mat4 CameraFps::getView() {

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

void CameraFps::processInput() {
	if (!firstTime) {
		yaw += (xPos - lastX) * sensitivity * DeltaTime;
		// ypos on computer ranges from bottom from top
		pitch += (lastY - yPos) * sensitivity * DeltaTime;
	}
	firstTime = false;
	lastX = xPos;
	lastY = yPos;
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	projection = glm::perspective(glm::radians(fov), (float) Width / Height, near, far);
	glm::vec3 cameraFront;
	cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(cameraFront);
	if (Window != nullptr) {
		if (glfwGetKey(Window, GLFW_KEY_LEFT_ALT)) {
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		} else {
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) {
			pos += cameraFront * cameraSpeed * DeltaTime;
		}
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
			pos -= cameraFront * cameraSpeed * DeltaTime;
		}
		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) {
			pos += glm::normalize(glm::cross(up, cameraFront)) * cameraSpeed * DeltaTime;
		}
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) {
			pos += glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed * DeltaTime;
		}
	}
	pos = pos;
	target = pos + cameraFront;
	up = up;
}

void CameraFps::CameraCallBackInit() {
	glfwSetCursorPosCallback(Window, mouse_callback);
	glfwSetScrollCallback(Window, scroll_callback);
}

void CameraFps::SetViewProjectMat(m_Shader* const m_shader) {
	m_shader->SetMat4fv("view", glm::value_ptr(getView()));
	m_shader->SetMat4fv("projection", glm::value_ptr(projection));
}

static void mouse_callback(GLFWwindow* Window, double xpos, double ypos) {
	xPos = xpos;
	yPos = ypos;
}

static void scroll_callback(GLFWwindow* Window, double xoffset, double yoffset) {
	fov -= yoffset;
	if (fov > 45.0f) {
		fov = 45.0f;
	}
	else if (fov < 1.0f) {
		fov = 1.0f;
	}
}