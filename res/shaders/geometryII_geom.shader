#version 330 core

/////////////////////////// triangle strip with different color /////////////////////
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vs_out{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
} vs[];

uniform float time;

float mag = 2.0;

// Use uniform buffer
layout(std140) uniform Camera
{
	mat4 projection;
	mat4 view;
};


out ge_out{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
} ge;



///////////////////////////////// Take away ////////////////////////
// Always take care of transformation in world space or view space
// transformation in clip space would result werid behavior


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

void main() {
	vec3 normal = getNormal();
	ge.Normal = normal;
	mat4 clipM = projection * view;

	ge.Pos = vec3(explode(vs[0].Pos, normal));
	gl_Position = clipM * vec4(ge.Pos, 1.0);
	ge.UV = vs[0].UV;
	EmitVertex();
	ge.Pos = vec3(explode(vs[1].Pos, normal));
	gl_Position = clipM * vec4(ge.Pos, 1.0);
	ge.UV = vs[1].UV;
	EmitVertex();
	ge.Pos = vec3(explode(vs[2].Pos, normal));
	gl_Position = clipM * vec4(ge.Pos, 1.0);
	ge.UV = vs[2].UV;
	EmitVertex();
	EndPrimitive();
}