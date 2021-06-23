#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 Normal;
out vec3 Pos;
out vec2 UV;

uniform mat3 normalM;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	
	// Not efficient to calculate normal matrix in shader, a better way is to calculate on the cpu side and then set normal uniform
	//Normal = mat3(transpose(inverse(model))) * normal;
	Normal = normalM * normal;
	Pos = vec3(model * vec4(pos, 1.0));
	UV = uv;
	gl_Position = projection * view * vec4(Pos.xyz, 1.0);
}