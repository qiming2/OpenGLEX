#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out VS_OUT{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
	mat3 TBN;
} vs;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform bool tuto;
uniform bool hardCode;
uniform bool normalized;
// we have two ways to get right
// light calculation:
// 1. transform every sampled normal from tangent space to world space
// 2. transform everything else from world space into tangent space

// first method might be more expensive since there will be extra cost for each
// frag color calculation

// we choose the first one
void main() {
	vs.Pos = vec3(model * vec4(pos, 1.0));
	vec3 B;
	vec3 T;
	if (tuto) {
		if (hardCode) {
			vs.Normal = normalize(transpose(inverse(mat3(model))) * vec3(0.0, 0.0, 1.0));
			T = normalize(transpose(inverse(mat3(model))) * vec3(1.0, 0.0, 0.0));

			B = normalize(transpose(inverse(mat3(model))) * vec3(0.0, 1.0, 0.0));
		}
		else {
			vs.Normal = normalize(transpose(inverse(mat3(model))) * normal);
			T = normalize(transpose(inverse(mat3(model))) * tangent);

			B = normalize(transpose(inverse(mat3(model))) * bitangent);
		}
		
	}
	else {
		if (hardCode) {
			vs.Normal = normalize(transpose(inverse(mat3(model))) * normal);
			T = normalize(transpose(inverse(mat3(model))) * vec3(1.0, 0.0, 0.0));

			B = normalize(transpose(inverse(mat3(model))) * vec3(0.0, 0.0, -1.0));
		}
		else {
			vs.Normal = normalize(transpose(inverse(mat3(model))) * normal);
			T = normalize(transpose(inverse(mat3(model))) * tangent);

			B = normalize(transpose(inverse(mat3(model))) * bitangent);
		}
	}

	vec3 N = vs.Normal;
	if (normalized) {
		T = normalize(T - dot(T, N) * N);
		B = cross(N, T);
	}
	
	
	vs.UV = uv;
	vs.TBN = mat3(T, B, N);
	gl_Position = projection * view * vec4(vs.Pos, 1.0);
}