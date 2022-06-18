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

uniform sampler2D raw_hdr_tex;

// convert pos to uv coord
// spherical to cartesian

const vec2 invAtan = vec2(0.1591, 0.3193);
// ???????????????????????????????
// weird conversion, need to dig in more
vec2 sample_spherical_map(vec3 v) {
	v = normalize(v);
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}


void main() {
	vec2 uv = sample_spherical_map(m_world_pos); // make sure to normalize the local pos
	vec3 color = texture(raw_hdr_tex, uv).rgb;

	out_frag = vec4(color, 1.0);
}