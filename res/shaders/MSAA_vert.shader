#version 330 core

// default shader does not use all attributes
// but just show what are available to us
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0);
}