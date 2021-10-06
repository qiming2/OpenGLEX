#version 330 core

out vec4 out_color;
in VS_OUT{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
	mat3 TBN;
} vs;

struct Light {
	vec3 intensity;
	vec3 dir;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform Light light;
uniform vec3 viewPos;


void main() {
	vec3 viewDir = viewPos - vs.Pos;
	vec3 obj_color = texture(texture_diffuse1, vs.UV).rgb;
	// transform sampled normal from tangent space to world space
	// since it is from 0 to 1, we want to map them back to -1 to 1
	vec3 normal = TBN * texture(texture_normal1, vs.UV).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	
	// normal light calculation
	vec3 ambient = vec3(0.2, 0.2, 0.2) * light.intensity;
	float dotN = max(dot(normal, -light.dir), 0.0);
	vec3 diffuse = dotN * light.intensity;

	vec3 halfDir = normalize(-light.dir + viewDir);
	dotN = max(dot(halfDir, normal), 0.0);
	vec3 specular = pow(dotN, 32) * light.intensity;

	out_color = vec4((ambient + diffuse + specular) * obj_color, 1.0);
}