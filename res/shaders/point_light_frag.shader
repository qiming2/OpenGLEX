#version 330 core

// since we are writing to both render targets (two color buffer)
// we need to specify which output colors need to go to which render target
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_brightness;

in VS_OUT{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
} vs_in;

struct PointLight{
	vec3 color;
	vec3 pos;
};
#define NUM_LIGHTS 4
uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform PointLight lights[NUM_LIGHTS];
const vec3 ambient = vec3(0.2);

vec3 phong_shade(int i) {
	PointLight light = lights[i];
	vec3 light_dir = light.pos - vs_in.Pos;
	// attenuation
	float dis = length(light_dir);
	light_dir = normalize(light_dir);
	vec3 view_dir = normalize(viewPos - vs_in.Pos);
	vec3 normal = normalize(vs_in.Normal);
	vec3 obj_color = texture(texture_diffuse1, vs_in.UV).rgb;
	vec3 ambient_color = ambient * obj_color;

	float dotN = max(dot(normal, light_dir), 0.0);
	vec3 diffuse_color = light.color * obj_color * dotN;


	vec3 halfDir = normalize(light_dir + view_dir);
	float dotH = max(dot(halfDir, normal), 0.0);
	vec3 specular_color = light.color * pow(dotH, 32) * obj_color;
	
	
	// hard code a soft attenuation
	return (ambient_color + diffuse_color + specular_color) / (dis * dis);
}

const vec3 channel_weight = vec3(0.2126, 0.7152, 0.0722);

void main() {
	vec3 ret = vec3(0.0);
	for (int i = 0; i < NUM_LIGHTS; i++) {
		ret += phong_shade(i);
	}
	//out_color = vec4(1.0, 1.0, 1.0, 1.0);
	out_color = vec4(ret, 1.0);
	// Calculate luminance
	float luminance = dot(ret, channel_weight);
	if (luminance > 1.0) {
		out_brightness = vec4(ret, 1.0);
	}
	else {
		out_brightness = vec4(0.0, 0.0, 0.0, 1.0);
	}
}