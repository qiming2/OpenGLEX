#version 330 core

in vec4 fragPos;

uniform vec3 lightPos;
uniform float far;

void main() {
	float dis = length(lightPos - fragPos.xyz);
	dis = dis / far;
	gl_FragDepth = dis;
}