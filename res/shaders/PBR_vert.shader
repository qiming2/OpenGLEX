#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out VS_IN{
	vec3 pos;
	vec3 normal;
	vec3 uv;
} vs_out;

void main() {
	vs_out.pos = vec3(model * vec4(pos, 1.0));
	// Since we know we would not ever sheer the object
	// we don't need to take transpose of inverse of the model matrix
	//vs_out.normal = transpose(inverse(mat3(model))) * normal;
	vs_out.normal = mat3(model) * normal;
	vs_out uv = uv;

	gl_Position = projection * view * vec4(vs_out.pos, 1.0);
}