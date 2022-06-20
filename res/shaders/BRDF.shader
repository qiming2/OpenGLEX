#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

out vec2 UV;

void main() {
	UV = uv;
	gl_Position = vec4(pos, 1.0);
}

#type - delim

out vec2 out_frag;
in vec2 UV;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// @Reference(qiming): http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammpersley(uint i, uint N) {
	return vec2(float(i) / float(N), RadicalInverse_VdC);
}

vec3 ImportanceSampleGGX(vec3 Xi, vec3 N, float roughness) {
	float a = rouhgness * roughness;
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1 - cosTheta * cosTheta);

	vec3 H;
	H.x = sinTheta * cos(phi);
	H.z = sinTheta * sin(phi);
	H.y = cosTheta;

	vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = normalize(cross(N, tangent));
	
	vec3 sample_vec = H.x * tangent + H.y * bitangent + H.z * N;
	return sample_vec;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
	float a = roughness;
	float k = (a * a) / 2.0;
	
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	
	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

void main() {
	float NdotV = UV.x;
	float roughness = UV.y;

	vec3 V;
	V.x = sqrt(1.0 - NdotV * NdotV);
	V.y = 0.0;
	V.z = NdotV;

	float scale = 0.0;
	float bias = 0.0;

	vec3 N = vec3(0.0, 0.0, 1.0);
	
	const uint SAMPLE_COUNT = 1024u;
	for (uint i = 0; i < SAMPLE_COUNT; ++i) {
		vec2 Xi = Hammerlsey(i, SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(Xi, N, roughness);
		vec3 L = normalize(2.0 * dot(H, V) * H - V);

		float NdotH = max(dot(N, H), 0.0);
		float NdotL = max(dot(N, L), 0.0);
		float VdotH = max(dot(V, H), 0.0);

		if (NdotL > 0.0) {
			float G = GeometrySmith(N, V, L, roughness);
			float G_Vis = (G * VdotH) / (NdotH * NdotV);
			float Fc = pow(1.0 - VdotH, 5.0);

			scale += (1.0 - Fc) * G_Vis;
			bias += Fc * G_Vis;

		}
	}

	scale /= (float)SAMPLE_COUNT;
	bias /= (float)SAMPLE_COUNT;
	out_frag = vec2(scale, bias);

}