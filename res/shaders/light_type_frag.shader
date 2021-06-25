#version 330 core

in vec3 Pos;
in vec3 Normal;
in vec2 UV;

out vec4 out_color;
void main() {
	out_color = vec4(normalize(Pos), 1.0);
}