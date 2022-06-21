#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

out vec2 TexCoords;

void main() {
	TexCoords = uv;
	gl_Position = vec4(pos, 1.0);
}

#type - delim
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D brdf_map;

void main() {
	vec2 color = texture(brdf_map, TexCoords).rg;
	FragColor = vec4(color, 0.0, 1.0);
}