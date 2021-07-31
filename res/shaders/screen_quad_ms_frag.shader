#version 330 core

out vec4 out_color;

uniform sampler2DMS texture_diffuse1MS;
uniform int samples;
uniform int width;
uniform int height;
in vec2 UV;

void main() {
	out_color = vec4(0.0);
	ivec2 tex;
	tex.x = int(width * UV.x);
	tex.y = int(height * UV.y);
	for (int i = 0; i < samples; i++) {
		out_color += texelFetch(texture_diffuse1MS, tex, i);
	}

	out_color = vec4(out_color.xyz / 4.0, 1.0);
}