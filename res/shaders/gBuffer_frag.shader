#version 330 core

out vec4 out_color;
// multiple outputs to multiple render target
layout(location = 0) out vec3 pos;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec4 albedo;


uniform sampler2D texture_diffuse1;

in VS_OUT{
	vec3 pos;
	vec3 normal;
	vec2 uv;
	float spec;
	} vs_in;

void main() {
	pos = vs_in.pos;
	normal = normalize(vs_in.normal);
	albedo.rgb = texture(texture_diffuse1, vs_in.uv).rgb;
	albedo.a = vs_in.spec;
}