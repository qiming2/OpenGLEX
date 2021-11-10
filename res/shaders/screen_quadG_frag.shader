#version 330 core

uniform sampler2D posBuf;
uniform sampler2D normalBuf;
uniform sampler2D albedoBuf;

out vec4 out_color;
in vec2 UV;

struct DirLight {
	vec3 dir;
	vec3 color;
};


uniform vec3 viewPos;

// one directional light
uniform DirLight light;
const vec3 ambient = vec3(0.1, 0.1, 0.1);
void main() {
	vec3 pos = texture(posBuf, UV).xyz;
	vec3 normal = normalize(texture(normalBuf, UV).xyz);
	vec3 albedo = texture(albedoBuf, UV).rgb;
	float spec = texture(albedoBuf, UV).a;
	// light calculation
	vec3 lightDir = normalize(-light.dir);
	vec3 viewDir = normalize(viewPos - pos);
	vec3 ambient = albedo * ambient;
	float dotN = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = dotN * light.color * albedo;


	vec3 halfDir = normalize(lightDir + viewDir);
	float dotH = pow(max(dot(halfDir, normal), 0.0), 16.0);
	vec3 specular = light.color * dotH * spec;
	
	out_color = vec4(ambient + diffuse + specular, 1.0);
	//out_color = vec4(albedo, 1.0);
}