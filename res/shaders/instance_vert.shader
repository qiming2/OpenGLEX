#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 offset;

out vec3 Color;

void main() {
	vec2 newP = pos * gl_InstanceID / 361.0;
	Color = color;
	gl_Position = vec4(newP + offset, 0.0, 1.0);
}