#version 330 core

in vec3 Pos;
in vec3 Normal;
in vec2 UV;

// Directional light
struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
};

out vec4 out_color;

uniform Material material;
uniform Light light;

void main() {
	// light dir
	vec3 dir = normalize(-light.direction);
	vec3 normal = normalize(Normal);
	// obj color
	vec3 objColor = vec3(texture(material.diffuse, UV));
	vec3 objSpecular = vec3(texture(material.specular, UV));

	// emission currently none

	// ambient
	vec3 ambientColor = light.ambient * objColor;
	// diffuse
	float diffuseF = max(dot(dir, normal), 0.0);
	vec3 diffuseColor = diffuseF * light.diffuse * objColor;

	// specular
	

	vec3 color = ambientColor + diffuseColor;
	out_color = vec4(color, 1.0);
}