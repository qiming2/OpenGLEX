#version 330 core

out vec4 out_color;
in vec3 texDir;

uniform samplerCube cubeMap;

void main() {
	out_color = texture(cubeMap, texDir);
}