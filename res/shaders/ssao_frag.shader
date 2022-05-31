#version 330 core

uniform sampler2D posBuf;
uniform sampler2D normalBuf;
uniform sampler2D albedoBuf;
uniform sampler2D texNoise;

out float out_color;
in vec2 UV;

uniform vec3 samples[64];
int kernelSize = 64;
float radius = 0.1;
float bias = 0.025;

uniform float width;
uniform float height;

uniform mat4 view;
uniform mat4 projection;

void main() {
	vec2 noiseScale = vec2(width / 4.0, height/4.0);

	// data from textures
	vec3 fragPos = vec3(view * vec4(texture(posBuf, UV).xyz, 1.0));
	vec3 normal = normalize(texture(normalBuf, UV).xyz);
	vec3 randVec = normalize(texture(texNoise, UV*noiseScale).xyz);

	// create tbn from tangent-space to world space
	// essentially this is to randomly rotate noise samples
	vec3 tangent = normalize(randVec - dot(randVec, normal) * normal);
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	vec3 sPos;
	vec4 screenPos;
	for (int i = 0; i < kernelSize; ++i) {
		////get sample position
		//sPos = TBN * samples[i];
		//// Sample sphere radius
		//sPos = pos + sPos * radius;
		//
		//// project onto screen and get depth info
		//screenPos = projection * view * vec4(sPos, 1.0);
		//screenPos.xyz /= screenPos.w;
		//screenPos.xyz = screenPos.xyz * 0.5 + 0.5;

		//float sDepth = (view * texture(posBuf, screenPos.xy)).z;

		//// Limit impact of ssao
		//float rangeCheck = smoothstep(0.0, 1.0, radius / abs(pos.z - sDepth));
		//occlusion += (sDepth >= sPos.z + bias ? 1.0 : 0.0) * rangeCheck;
		 // get sample position
		vec3 samplePos = TBN * samples[i]; // from tangent to view-space
		samplePos = fragPos + samplePos * radius;

		// project sample position (to sample texture) (to get position on screen/texture)
		vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset; // from view to clip-space
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

		// get sample depth
		float sampleDepth = texture(posBuf, offset.xy).z; // get depth value of kernel sample

		// range check & accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / kernelSize);
	//out_color = occlusion;
	out_color = fragPos.z;
	//out_color = fragPos.x;
}