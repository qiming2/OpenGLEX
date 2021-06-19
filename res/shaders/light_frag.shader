#version 330 core

out vec4 out_color;
in vec3 Normal;
in vec3 Pos;
in vec2 UV;

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
	vec3 lightDir = normalize(lightPos - Pos);
	vec3 normal = normalize(Normal);

	// Ambient
	vec3 ambientColor = 0.2 * lightColor;

	// Diffuse
	float diffF = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diffF * lightColor;

	// Specular
	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = normalize(viewPos - Pos);
	int shinness = 64;
	float specularF = pow(max(dot(reflectDir, viewDir), 0.0), shinness);
	vec3 specularColor = specularF * lightColor * specularStrength;

	vec3 color = vec3(texture(texture1, UV));
	//vec3 color = vec3(1.0);
	color = color * (diffuse + ambientColor + specularColor);
	out_color = vec4(color.rgb, 1.0);
}