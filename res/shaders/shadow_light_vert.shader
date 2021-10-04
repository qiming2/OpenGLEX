#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 n;
layout(location = 2) in vec2 uv;

out VS_OUT {
	vec3 Pos;
	vec2 UV;
	vec3 N;
	vec4 lightSpacePos;
} out_data;

uniform mat4 lightProjectView;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
	out_data.Pos = vec3(model * vec4(pos, 1.0));
	out_data.lightSpacePos = lightProjectView * vec4(out_data.Pos, 1.0);
	out_data.UV = uv;
	out_data.N = mat3(model) * n;
	gl_Position = projection * view * vec4(out_data.Pos, 1.0);
}