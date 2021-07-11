#version 330 core

in vec3 Pos;
in vec2 UV;
in vec3 Normal;
out vec4 out_color;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

void main() {
	/*vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - Pos);*/
	vec4 tex_color = texture(texture_diffuse1, UV);
	if (tex_color.a < 0.1) {
		discard;
	}
	out_color = tex_color;
	
	//out_color = vec4(1.0, 1.0, 1.0, 1.0);
	/*if (dot(viewDir, normal) < 0.10) {
		out_color = vec4(0.0, 0.0, 0.2, 1.0);
	}*/
}