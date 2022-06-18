#version 330 core

layout(location = 0) in vec3 pos;

out vec3 texDir;

uniform mat4 view;
uniform mat4 projection;

void main() {
	texDir = pos;

	mat4 rot_view = mat4(mat3(view));
	vec4 raw_pos = projection * rot_view * vec4(pos, 1.0);

	gl_Position = raw_pos.xyww;
}

#type - delim

#version 330 core

out vec4 out_color;
in vec3 texDir;

uniform samplerCube env_map;

void main() {
	vec3 env_color = texture(env_map, texDir).rgb;

	// Tone mapping
	env_color = env_color / (env_color + vec3(1.0));
	env_color = pow(env_color, vec3(1.0 / 2.2));
	out_color = vec4(env_color, 1.0);
	gl_FragDepth = 1.0;
}