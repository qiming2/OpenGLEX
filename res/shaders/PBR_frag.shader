#version 330 core
out vec4 out_color;
in VS_IN{
	vec3 pos;
	vec3 normal;
	vec2 uv;
} vs_in;
// Material Params
uniform sampler2D albedo_tex;
//uniform sampler2D normal_tex;
uniform sampler2D metallic_tex;
uniform sampler2D roughness_tex;

uniform vec3 albedo_test;
uniform float roughness_test;
uniform float metallic_test;
uniform float ao;

// Lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

const float PI = 3.14159265359;

uniform vec3 camPos;

uniform bool use_tex;
// ------------------------------ The Rendering Equations -------------------------------
// Normal distribution approximates the relative surface area of microfacets that align tot the half way vector h

// Trowbridge-Reitz GGX

// f(n, h, roughness) = a^2/ (PI * (dot(n, h)^2 * (a^2 - 1) +1)^2)
float NormalDistributionGGX(vec3 n, vec3 h, float r) {
	float a = r * r;
	float a2 = a * a;
	float ndoth = max(dot(n, h), 0.0);
	float ndoth2 = ndoth * ndoth;

	float denom_right = ndoth2 * (a2 - 1.0) + 1.0;
	float denom = PI * denom_right * denom_right;
	
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

float GeometrySmith(vec3 n, vec3  v, vec3 l, float roughness) {
	// direct lighting thus using kdirect func
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;

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




// Main components that connect everything together
void main() {
	// Texture Material PBR
	// Add texture lookup here
	vec3 albedo;
	float roughness;
	float metallic;
	if (use_tex) {
		albedo = pow(texture(albedo_tex, vs_in.uv).rgb, vec3(2.2));
		roughness = texture(roughness_tex, vs_in.uv).r;
		metallic = texture(metallic_tex, vs_in.uv).r;
	}
	else {
		albedo = albedo_test;
		roughness = roughness_test;
		metallic = metallic_test;
	}
	


	vec3 N = normalize(vs_in.normal);
	vec3 V = normalize(camPos - vs_in.pos);
	
	// Pre-compute F0 param
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// reflectance equation calculating diff and spec color
	vec3 light_val = vec3(0.0);

	// Add up individual point light radiance contribution
	for (int i = 0; i < 4; ++i) {
		vec3 L = normalize(lightPositions[i] - vs_in.pos);
		vec3 H = normalize(L + V);
		float dist = length(lightPositions[i] - vs_in.pos);
		// inverse of square dist attenuation for point light
		float attenuation = 1.0 / (dist * dist);
		vec3 light_radiance = attenuation * lightColors[i];

		// Cook Torrance BRDF

		// DFG
		float D = NormalDistributionGGX(N, H, roughness);
		// Direct lighting k
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 numerator = D * F * G;
		float deno = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // prevent dividing by zero
		vec3 specular = numerator / deno;

		// Fresnel reflectance indice is specular param
		vec3 ks = F;

		// In order to maintain the conservation of energy, kd is set to 1.0 - F0
		// unless the object emits light
		vec3 kd = vec3(1.0) - ks;
		
		// Only non-metals or partial metals should have diffuse light, pure metal does not have any diffuse color
		kd *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);

		light_val += (kd * albedo / PI + specular) * light_radiance * NdotL;
		
	}
	F = 
	// Will change this when learning IBL: image based lighting
	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 color = ambient + light_val;
	
	// tone mapping since we are doing all the calculation in hdr
	color = color / (color + vec3(1.0));

	// gamma correction since we want all the calculations are linear
	color = pow(color, vec3(1.0 / 2.2));
	
	//vec3 color_temp = vec3(1.0);
	out_color = vec4(color, 1.0);
}