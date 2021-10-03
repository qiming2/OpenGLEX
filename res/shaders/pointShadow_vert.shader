#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 model;

// Only need the world space position
void main() {
	gl_position = model * vec4(pos, 1.0);
}