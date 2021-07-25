#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;

out vs_out{

	vec3 Pos;
	vec3 Normal;
	vec2 UV;
} vs;

void main() {
	vs.Pos = vec3(model * vec4(pos, 1.0));
	vs.Normal = transpose(inverse(mat3(model))) * normal;
	vs.UV = uv;
	//gl_Position = projection * view * vec4(vs.Pos, 1.0);
}