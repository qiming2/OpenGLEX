#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 uv;

out vec2 UV;
uniform mat4 model;

void main() {
	UV = uv;
	vec4 newP = model * vec4(pos, 1.0);
	gl_Position = vec4(newP.x, newP.y, 0.0, 1.0);
}