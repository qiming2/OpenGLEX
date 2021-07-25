#version 330 core

out vec4 out_color;

in ge_out{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
} ge_in;

void main() {
	out_color = vec4(0.0, 0.4, 0.7, 1.0);
}