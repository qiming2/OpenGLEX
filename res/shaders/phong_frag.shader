#version 330 core

out vec4 out_color;
in vec3 Normal;
in vec3 Pos;
in vec2 UV;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D texture1;
uniform Light light;
uniform vec3 viewPos;
uniform Material material;

void main() {
	vec3 lightDir = normalize(light.pos - Pos);
	vec3 normal = normalize(Normal);

	// Ambient
	vec3 ambientColor = material.ambient * light.ambient;

	// Diffuse
	float diffF = max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor = diffF * light.diffuse * material.diffuse;

	// Specular
	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = normalize(viewPos - Pos);
	int shinness = 64;
	float specularF = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specularColor = specularF * light.specular * material.specular;

	//vec3 color = vec3(texture(texture1, UV));
	//vec3 color = vec3(1.0);
	vec3 color = diffuseColor + ambientColor + specularColor;
	out_color = vec4(color.rgb, 1.0);
}

