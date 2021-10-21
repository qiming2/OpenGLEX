#version 330 core
out vec4 out_color;
in vec2 UV;
uniform sampler2D texture_hdr;
uniform sampler2D texture_brightness;
uniform bool hdr;
// exposure
uniform float exposure;
const float gamma = 2.2;
void main() {
	// gamma correct before tone mapping
	vec3 hdr_color = texture(texture_hdr, UV).rgb + texture(texture_brightness, UV).rgb;
	if (hdr) {
		// Several tone mapping
		// reinhard that balances out the brightness through all color channel
		/*hdr_color = hdr_color / (hdr_color + vec3(1.0));
		hdr_color = pow(hdr_color, vec3(1.0 / 2.2));*/

		// exposure
		hdr_color = vec3(1.0) - exp(-hdr_color * exposure);
		hdr_color = pow(hdr_color, vec3(1.0 / 2.2));
	}
	else {
		hdr_color = pow(hdr_color, vec3(1.0 / 2.2));
	}

	out_color = vec4(hdr_color, 1.0);
}