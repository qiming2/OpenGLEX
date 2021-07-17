#version 330 core

layout(location = 0) in vec3 pos;

out vec3 texDir;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	texDir = pos;

	vec4 real_pos = projection * view * model * vec4(pos, 1.0);
	gl_Position = real_pos.xyww;
}