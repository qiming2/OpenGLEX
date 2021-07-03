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

// point light
struct PointLight {
	vec3 pos;

	vec3 ambient;
	vec3 specular;
	vec3 diffuse;

	float constantAtten;
	float linearAtten;
	float quadraticAtten;
};

// Spot light
struct SpotLight {
	vec3 pos;
	vec3 direction;
	float cutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 specular;
	vec3 diffuse;

	float constantAtten;
	float linearAtten;
	float quadraticAtten;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
};

out vec4 out_color;

uniform float shininess;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform PointLight pLight;
uniform SpotLight sLight;
uniform int usePointLight;
uniform int useDirectionalLight;
uniform int useSpotLight;

vec3 dir;
vec3 normal;
vec3 objColor;
vec3 objSpecular;
vec3 lightAmb;
vec3 lightDiff;
vec3 lightSpe;

vec3 phong_shading() {
	// emission currently none

	// ambient
	vec3 ambientColor = lightAmb * objColor;
	// diffuse
	float diffuseF = max(dot(dir, normal), 0.0);
	vec3 diffuseColor = diffuseF * lightDiff * objColor;

	// specular
	vec3 viewDir = normalize(viewPos - Pos);
	vec3 reflectDir = reflect(-dir, normal);
	float specF = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specularColor = lightSpe * objSpecular * specF;
	return ambientColor + diffuseColor + specularColor;
}

void main() {
	// light dir
	
	normal = normalize(Normal);
	// obj color
	objColor = vec3(texture(material.diffuse, UV));
	objSpecular = vec3(texture(material.specular, UV));
	vec3 color = vec3(0.0, 0.0, 0.0);

	///////////////////////////////////////// Directional Light/////////////////////////
	if (useDirectionalLight != 0) {
		dir = normalize(-light.direction);
		lightAmb = light.ambient;
		lightDiff = light.diffuse;
		lightSpe = light.specular;
		color += phong_shading();
	}
	

	///////////////////////////////////////// Point light /////////////////////////////

	// Point light attenuation
	if (usePointLight != 0) {
		float distance = length(pLight.pos - Pos);
		float atten = 1.0 / (pLight.constantAtten + pLight.linearAtten * distance + pLight.quadraticAtten * distance * distance);

		// the rest computation is the same as except that the final color needs to multiply a attenuation value

		dir = normalize(pLight.pos - Pos);
		lightAmb = pLight.ambient;
		lightDiff = pLight.diffuse;
		lightSpe = pLight.specular;
		color += phong_shading() * atten;
	}
	

	///////////////////////////////////////// Spot light ///////////////////////////
	
	if (useSpotLight != 0) {
		vec3 realLightDir = normalize(-sLight.direction);
		vec3 toFragDir = normalize(sLight.pos - Pos);
		float eps = sLight.cutoff - sLight.outerCutoff;
		float theta = dot(realLightDir, toFragDir);
		float intensity = clamp((theta - sLight.outerCutoff) / eps, 0.0, 1.0);
			// Within spotlight range, do spotlight calculation

		float distance = length(sLight.pos - Pos);
		float atten = 1.0 / (sLight.constantAtten + sLight.linearAtten * distance + sLight.quadraticAtten * distance * distance);
		dir = normalize(sLight.pos - Pos);
		lightAmb = sLight.ambient;
		lightDiff = sLight.diffuse;
		lightSpe = sLight.specular;
		color += phong_shading() * atten * intensity;
	}
	out_color = vec4(color, 1.0);
}