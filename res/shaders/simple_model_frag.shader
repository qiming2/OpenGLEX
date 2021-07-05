#version 330 core

in vec2 UV;
out vec4 out_color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

void main() {
	out_color = texture(texture_diffuse1, UV);
	//out_color = vec4(1.0, 1.0, 1.0, 1.0);
}