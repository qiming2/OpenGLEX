#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

out vec2 UV;

void main() {
	// NDC coordinate
	// opengl would divde xyz with w which we
	// can set it to 1.0
	// we don't really need to care about
	// depth value for now
	gl_Position = vec4(pos, 1.0);
	UV = uv;
}