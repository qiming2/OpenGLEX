#pragma once
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include "GeometryUtil.h"
#include "FileUtil.h"
#define M_PI 3.14159265358979323846
extern GLFWwindow* Window;
extern float DeltaTime;
extern int Width;
extern int Height;
extern Renderer gl_renderer;
static std::ostream& operator<<(std::ostream& os, glm::vec4 const& vec) {
	return os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z << " w: " << vec.w;
}

static std::ostream& operator<<(std::ostream& os, glm::vec3 const& vec) {
	return os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z;
}