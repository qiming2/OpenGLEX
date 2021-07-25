#version 330 core

/////////////////////////// triangle strip with different color /////////////////////
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vs_out{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
} vs[];

uniform float time;

float mag = 2.0;
float len = 0.5;

// Use uniform buffer
layout(std140) uniform Camera
{
	mat4 projection;
	mat4 view;
};


mat4 clipM;
vec3 normal;


out ge_out{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
} ge;

//////////// Simple Explosion with geometry shader
vec3 getNormal() {
	vec3 ab = vec3(vs[1].Pos - vs[0].Pos);
	vec3 bc = vec3(vs[2].Pos - vs[1].Pos);
	return normalize(cross(ab, bc));
}

vec4 explode(vec3 position, vec3 normal) {
	vec3 direction = normal * ((sin(time) + 1) / 2.0) * mag;
	return vec4(position, 1.0) + vec4(direction, 0.0);
}

void generateLine(vec3 position, vec3 anormal) {
	gl_Position = clipM * vec4(position, 1.0);
	EmitVertex();
	gl_Position = clipM * vec4(position + len * anormal, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main() {
	normal = getNormal();
	clipM = projection * view;

	ge.Pos = vec3(explode(vs[0].Pos, normal));
	generateLine(ge.Pos, normalize(vs[0].Normal));
	ge.Pos = vec3(explode(vs[1].Pos, normal));
	generateLine(ge.Pos, normalize(vs[1].Normal));
	ge.Pos = vec3(explode(vs[2].Pos, normal));
	generateLine(ge.Pos, normalize(vs[2].Normal));
}