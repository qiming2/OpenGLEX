#version 330 core

out vec4 gl_Color;
in vec2 UV;

uniform sampler2D texture1;
uniform vec3 lightColor;

void main() {
	vec4 color = texture(texture1, UV);
	gl_Color = vec4(color.rgb * lightColor, color.a);
}