#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VS_OUT{
	vec3 pos;
	vec3 normal;
	vec2 uv;
	float spec;
} vs_out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
	vs_out.pos = vec3(model * vec4(pos, 1.0));
	vs_out.normal = transpose(inverse(mat3(model))) * normal;
	vs_out.uv = uv;
	// hard code spec color
	vs_out.spec = 0.5;
	gl_Position = projection * view * vec4(vs_out.pos, 1.0);
}