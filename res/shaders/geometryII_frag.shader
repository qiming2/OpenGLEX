#version 330 core

out vec4 out_color;

in ge_out{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
} ge_in;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main() {
	out_color = texture(texture_diffuse1, ge_in.UV);
}