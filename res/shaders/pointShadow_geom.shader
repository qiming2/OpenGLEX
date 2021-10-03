#version 330 core

/////////////////////////// triangle strip with different color /////////////////////
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrixs[6];


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