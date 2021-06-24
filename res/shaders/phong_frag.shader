#version 330 core

out vec4 out_color;
in vec3 Normal;
in vec3 Pos;
in vec2 UV;

struct Material
{
	sampler2D texture_container;
	sampler2D texture_specular;
	sampler2D texture_emission;
	float shininess;
};

struct Light
{
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define PI 3.1415926538
uniform float u_time;
uniform Light light;
uniform vec3 viewPos;
uniform Material material;

void main() {
	vec3 lightDir = normalize(light.pos - Pos);
	vec3 normal = normalize(Normal);
	vec3 objColor = vec3(texture(material.texture_container, UV));
	// Emission
	vec3 emissionColor = texture(material.texture_emission, UV).rgb;

	// Ambient
	vec3 ambientColor = objColor * light.ambient;

	// Diffuse
	float diffF = max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor = diffF * light.diffuse * objColor;

	// Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = normalize(viewPos - Pos);
	float specularF = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specularColor = specularF * light.specular * vec3(texture(material.texture_specular, UV)); 

	//vec3 color = vec3(texture(texture1, UV));
	//vec3 color = vec3(1.0);
	vec3 color = ambientColor + diffuseColor + specularColor;
	if (vec3(texture(material.texture_specular, UV)) == vec3(0.0))
	{
		
		/*float factor = max(sin(u_time * 3.0 + UV.y * PI), 0.0);
		color += emissionColor * pow(factor, 10);*/

		// want to use the border but change the picture for the wood portion
		color += emissionColor * light.ambient + emissionColor * diffF * light.diffuse - diffuseColor - ambientColor;
		
	}
	out_color = vec4(color.rgb, 1.0);
}

