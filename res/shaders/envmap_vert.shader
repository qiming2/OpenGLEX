#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out vec3 Pos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
	Pos = vec3(model * vec4(pos, 1.0));
	Normal = mat3(transpose(inverse(model))) * normal;

	gl_Position = projection * view * vec4(Pos, 1.0);
}