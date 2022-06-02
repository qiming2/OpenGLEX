#version 330 core

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

uniform bool useSSAO;
out vec4 out_color;
in vec2 UV;

struct DirLight {
	vec3 dir;
	vec3 color;
};


uniform vec3 viewPos;

// one directional light
uniform DirLight light;
const vec3 ambient = vec3(0.3, 0.3, 0.3);
void main() {
	vec3 pos = texture(gPosition, UV).xyz;
	vec3 normal = normalize(texture(gNormal, UV).xyz);
	vec3 albedo = texture(gAlbedo, UV).rgb;
	float SSAO_F = useSSAO ? texture(ssao, UV).r : 1.0;


	float spec = texture(gAlbedo, UV).a;
	// light calculation
	vec3 lightDir = normalize(-light.dir);
	vec3 viewDir = normalize(viewPos - pos);
	vec3 ambient = albedo * ambient * SSAO_F;
	float dotN = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = dotN * light.color * albedo;


	vec3 halfDir = normalize(lightDir + viewDir);
	float dotH = pow(max(dot(halfDir, normal), 0.0), 16.0);
	vec3 specular = light.color * dotH * spec;
	
	out_color = vec4((ambient + diffuse + specular), 1.0);
	//out_color = vec4(SSAO_F, SSAO_F, SSAO_F, 1.0);
	//out_color = vec4(albedo, 1.0);
}