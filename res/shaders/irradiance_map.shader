#version 330 core

layout(location = 0) in vec3 pos;

out vec3 m_world_pos;

uniform mat4 projection;
uniform mat4 view;

void main() {
	m_world_pos = pos;

	gl_Position = projection * view * vec4(pos, 1.0);
}

#type - delim

#version 330 core

out vec4 out_frag;

in vec3 m_world_pos;

uniform samplerCube env_map;

// accuracy partly depends on sample_delta
// the smaller it is, the more samples we take and
// the more accurate the approximation of irradiance value will be
const float sample_delta = 0.025;
const float PI = 3.14159265358979323846;
void main() {
	vec3 n = normalize(m_world_pos); // make sure to normalize the local pos
	vec3 irradiance = vec3(0.0);

	//Note(qiming): up and normal might be the same?
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, n));
	up = normalize(cross(n, right));
	float sample_count = 0.0;
	vec3 rotate_vec = vec3(0.0);
	vec3 sample_vec = vec3(0.0);
	for (float phi = 0.0; phi < 2 * PI; phi += sample_delta) {
		for (float theta = 0.0; theta < 0.5 * PI; theta += sample_delta) {
			rotate_vec = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

			sample_vec = rotate_vec.x * right + rotate_vec.y * up + rotate_vec.z * n;
			
			// cos(theta): How much light direction deviates from normal
			// sin(theta): Since the higher the hemisphere, the smaller the sample area becomes
			// thus we scale the contribution by sin(theta) as well
			irradiance += texture(env_map, sample_vec).rgb * cos(theta) * sin(theta);

			sample_count++;
		}
	}
	
	// Average over the samples
	// Riemann sum, can dig into more about the math
	irradiance = irradiance * PI * 1.0 / sample_count;

	out_frag = vec4(irradiance, 1.0);


}