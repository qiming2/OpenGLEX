#version 330 core

out vec4 gl_color;
in VS_IN{
	vec3 pos;
	vec3 normal;
	vec3 uv;
} vs_in;
// Material Params
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// Lights
uniform vec3 lightPosition[4];
uniform vec3 lightColors[4];

const float PI = 3.14159265359;

// ------------------------------ The Rendering Equations -------------------------------
// Normal distribution approximates the relative surface area of microfacets that align tot the half way vector h

// Trowbridge-Reitz GGX

// f(n, h, roughness) = a^2/ (PI * (dot(n, h)^2 * (a^2 - 1) +1)^2)
float NormalDistributionGGX(vec3 n, vec3 h, float r) {
	float a2 = a * a;
	float ndoth = max(dot(n, h), 0.0);
	float ndoth2 = ndoth * ndoth;

	float denom_right = ndoth2 * (a2 - 1.0) + 1.0;
	float denom = PI + denom_right * denom_right;

	return a2 / denom;
}

// Geometry function approximates the relative surface area in which microfacets overshadow each other

// Direct lighting		k = (roughness + 1)^2 / 8
// Image Based Lighting k = roughness^2 / 2
// f(n, v, k) = dot(n, v) / (dot(n, v) * (1 - k) + k)

// In order to get a good approximation, we take into account both light direction and view direction G(n, v, k) * G(n, l, k)
float SchlickGGX(float dotP, float k) {
	return dotP / (dotP * (1.0 - k) + k);
}

float GeometrySmith(vec3 n, vec3  v, vec3 l, float k) {
	float ndotv = max(dot(n, v), 0.0);
	float ndotl = max(dot(n, l), 0.0);

	float nvggx = SchlickGGX(ndotv, k);
	float nlggx = SchlickGGX(ndotl, k);

	return nvggx * nlggx;
}

// Fresnel Equation approximates the ratio of light that gets reflected over the light that gets refracted
// Fresnel effect is very common in our daily life, which describes a phenomenon that if viewing a surface from a steep angle, reflections become more apparent to the surface's base reflectivity

// Since F0 is tinted for some metal objects and are just a single number
// we usually pre-compute F0 to a vec3 components and have a metalness parameter to denote
// whether it is a metallic or non-metallic surface

// metalness ranges from 0 to 1 since we might want sand(non-metallic) on a metallic surface, which could be hard to render

// Pre-compute F0
// vec3 F0 = vec3(0.04); // since most dielectrics have base reflectivity around this value
// F0 = mix(F0, surface_rgb, metalness)

// Fresnel Schlick Equation
// f(h, v, F0) = F0 + (1-F0)*(1-dot(h, v))^5

vec3 FresnelSchlick(float costheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - costheta, 5.0);
}


void main() {


	gl_color = vec4(1.0);
}