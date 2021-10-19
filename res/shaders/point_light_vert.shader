#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VS_OUT{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
} vs_out;

uniform bool inverse;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vs_out.Pos = vec3(model * vec4(pos, 1.0));
	if (inverse) {
		vs_out.Normal = mat3(model) * -normal;
	}
	else {
		vs_out.Normal = mat3(model) * normal;
	}
	
	vs_out.UV = uv;
	gl_Position = projection * view * vec4(vs_out.Pos, 1.0);
}