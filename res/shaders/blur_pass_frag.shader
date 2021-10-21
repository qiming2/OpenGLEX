#version 330 core

out vec4 out_color;

uniform sampler2D texture_hdr1;
uniform bool horizontal;
in vec2 UV;
const float gaussian[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
	vec3 ret = texture(texture_hdr1, UV).rgb * gaussian[0];
	// calculate offset of texel
	// param: texture, mipmap level
	vec2 offset = 1.0 / textureSize(texture_hdr1, 0);
	if (horizontal) {
		for (int i = 1; i < 5; ++i) {
			ret += texture(texture_hdr1, UV + vec2(offset.x * i, 0.0)).rgb * gaussian[i];
			ret += texture(texture_hdr1, UV - vec2(offset.x * i, 0.0)).rgb * gaussian[i];
		}
	}
	else {
		for (int i = 1; i < 5; ++i) {
			ret += texture(texture_hdr1, UV + vec2(0.0, offset.y * i)).rgb * gaussian[i];
			ret += texture(texture_hdr1, UV - vec2(0.0, offset.y * i)).rgb * gaussian[i];
		}
	}
	out_color = vec4(ret, 1.0);
}