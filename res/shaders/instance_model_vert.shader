#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 5) in mat4 model_m;

out vec3 Pos;
out vec3 Normal;
out vec2 UV;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main() {
	UV = uv;
	mat4 newModel = model * model_m;
	Pos = vec3(newModel * vec4(pos, 1.0));

	Normal = mat3(transpose(inverse(newModel))) * normal;

	gl_Position = projection * view * vec4(Pos, 1.0);
}