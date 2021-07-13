#version 330 core

out vec4 out_color;

uniform sampler2D texture_diffuse1;

in vec2 UV;

// Kernel type to choose
uniform int type;

// Make an array of indexes to make it easier to query from texture
uniform float width;
uniform float height;
float offset_w = 1.0 / width;
float offset_h = 1.0 / height;
vec2 kernel_off[9];


vec3 calculateKernel(float kernel[9]) {
	vec3 sum = vec3(0, 0, 0);
	
	for (int i = 0; i < 9; i++) {
		sum += vec3(texture(texture_diffuse1, kernel_off[i])) * kernel[i];
	}
	return sum;
}


int blur_t = 1 << 0;
int edge_t = 1 << 1;
int sharpen_t = 1 << 2;

void main() {
	vec3 color = vec3(0, 0, 0);


	kernel_off = vec2[9](UV + vec2(-offset_w, offset_h), UV + vec2(0, offset_h), UV + vec2(offset_w, offset_h), UV + vec2(-offset_w, 0), UV + vec2(0, 0), UV + vec2(offset_w, 0), UV + vec2(-offset_w, -offset_h), UV + vec2(0, -offset_h), UV + vec2(offset_w, -offset_h));
	offset_w = 1.0 / width;
	offset_h = 1.0 / height;

	// Many postprocessing effects can be implemented here

	///////////// Inversion //////////
 
	//out_color = vec4(1.0 - color, 1.0);

	///////////// Gray Scale ////////
	// Simple grayscale
 
	//float sum = (color.r + color.g + color.b) / 3.0;
	//out_color = vec4(vec3(sum), 1.0);

	///////////////////////////////////////////////
 
	// Physically based grayscale since human eyes are
	// sensitive to green colors and least to blue
	// 
	//float sum = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	//out_color = vec4(vec3(sum), 1.0);

	/////////// Kernel /////////////

	// 1. Sharpen
	float sharpen[9] = float[](
		2, 2, 2,
		2, -15, 2,
		2, 2, 2
	);

	// 2. blur, there are a lot of kernels that do this
	// just pick a simple gaussian one
	float gaussian[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);

	float normalBlur[9] = float[](
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9
	);


	// 3. Edge detection

	float edge[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
	);

	float edge2[9] = float[](
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	);

	if (type == blur_t) {
		color = calculateKernel(gaussian);
	} else if (type == edge_t) {
		color = calculateKernel(edge2);
	} else if (type == sharpen_t) {
		color = calculateKernel(sharpen);
	} else {
		color = texture(texture_diffuse1, UV).rgb;
	}

	
	out_color = vec4(color, 1.0);
}