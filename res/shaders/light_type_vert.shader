#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 Pos;
out vec3 Normal;
out vec2 UV;

uniform mat3 normalM;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	// Transform pos to world position
	Pos = vec3(model * vec4(pos.xyz, 1.0));

	// normalM = mat3(transpose(inverse(model)))
	// compute normal matrix in shader is expensive
	// so we send it here after calculating it
	Normal = normalM * normal;
	UV = uv;

	gl_Position = projection * view * vec4(Pos.xyz, 1.0);
}