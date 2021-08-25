#version 330 core

out vec4 out_color;
in vec2 UV;

uniform sampler2D texture1;

void main() {
	float depth = texture(texture1, UV).r;
	out_color = vec4(depth, depth, depth, 1.0);
}