#version 330 core

out vec4 out_color;

uniform vec3 lightColor;

void main()
{

	out_color = vec4(lightColor, 1.0);
}