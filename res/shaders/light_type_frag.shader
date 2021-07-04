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
// four point lights
#define NR_POINT_LIGHTS 4
uniform PointLight pLight[NR_POINT_LIGHTS];

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

vec3 phong_shading();

// light_dir: intuitive light direction, the function will inverse it if necessary
vec3 calDirectionalLight(Light light) {
	dir = normalize(-light.direction);
	lightAmb = light.ambient;
	lightDiff = light.diffuse;
	lightSpe = light.specular;
	return phong_shading();
}

vec3 calPointLight(PointLight light) {
	float distance = length(light.pos - Pos);
	float atten = 1.0 / (light.constantAtten + light.linearAtten * distance + light.quadraticAtten * distance * distance);

	dir = normalize(light.pos - Pos);
	lightAmb = light.ambient;
	lightDiff = light.diffuse;
	lightSpe = light.specular;
	// output color needs to multiply by a attenuation factor
	return phong_shading() * atten;
}

vec3 calSpotLight(SpotLight light) {
	vec3 realLightDir = normalize(-light.direction);
	vec3 toFragDir = normalize(light.pos - Pos);

	// Outer cutoff for soft edges
	float eps = light.cutoff - light.outerCutoff;
	float theta = dot(realLightDir, toFragDir);

	// Within spotlight range, do spotlight calculation, but we can just use a clamp to
	// simulate cutoff effect
	float intensity = clamp((theta - light.outerCutoff) / eps, 0.0, 1.0);
	

	float distance = length(light.pos - Pos);
	float atten = 1.0 / (light.constantAtten + light.linearAtten * distance + light.quadraticAtten * distance * distance);
	dir = normalize(light.pos - Pos);
	lightAmb = light.ambient;
	lightDiff = light.diffuse;
	lightSpe = light.specular;
	return phong_shading() * atten * intensity;
}

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
		color += calDirectionalLight(light);
	}
	

	///////////////////////////////////////// Point light /////////////////////////////

	// Point light attenuation
	if (usePointLight != 0) {
		for (int i = 0; i < NR_POINT_LIGHTS; i++) {
			color += calPointLight(pLight[i]);
		}
	}
	

	///////////////////////////////////////// Spot light ///////////////////////////
	
	if (useSpotLight != 0) {
		color += calSpotLight(sLight);
	}
	out_color = vec4(color, 1.0);
}