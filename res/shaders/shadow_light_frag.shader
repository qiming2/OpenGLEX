#version 330 core

out vec4 out_color;

in VS_OUT {
	vec3 Pos;
	vec2 UV;
	vec3 N;
	vec4 lightSpacePos;
} in_data;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture1;


// currently hardcode light properties
// purpose of this shader is to demonstrate basic
// shadow implementation
vec3 lightAmb;
vec3 lightDiff;
vec3 lightSpe;
float shininess;
uniform vec3 dir;
uniform vec3 viewPos;
uniform mat4 lightProjectView;
vec3 objColor;

vec3 normal;
vec3 Pos;
vec2 UV;

float isInShadow() {
	vec4 lightSpacePos = in_data.lightSpacePos;
	lightSpacePos = lightSpacePos / lightSpacePos.w;
	// since after perspective division
	// xyz are all in range from -1 to 1
	// however depth map ranges from 0 to 1
	// we need to remap z to the correct space
	lightSpacePos = lightSpacePos * 0.5 + 0.5;
	float depth = texture(texture1, vec2(lightSpacePos.x, lightSpacePos.y)).r;
	
	// Shadow Acne, if resolution is not big enough, multiple
	// fragments can sample the same value from the depth map
	// thus we add a small bias to it
	// we can make the bias change based on light
	// direction and normal val of current fragment
	// this way, we consider the situations that light angles are
	// changing

	// However too much bias can introduce an issue called
	// peter panning which the shadow looks detached from
	// the actual object, usually tight

	// pcf: percentage-closer filtering
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(texture1, 0);
	float curD = 0.0;
	float bias = max(0.05 * (1.0 - dot(-dir, normal)), 0.005);
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			curD = texture(texture1, lightSpacePos.xy + vec2(i, j) * texelSize).r;
			shadow += curD + bias < lightSpacePos.z ? 0.0 : 1.0;
		}
	}
	
	return shadow / 9.0;
}

vec3 phong_shading() {
	// emission currently none

	// ambient
	vec3 ambientColor = lightAmb * objColor;
	// diffuse
	float diffuseF = max(dot(-dir, normal), 0.0);
	vec3 diffuseColor = diffuseF * lightDiff * objColor;

	// specular
	vec3 viewDir = normalize(viewPos - Pos);
	vec3 halfDir = normalize(-dir + viewDir);
	float specF = pow(max(dot(normal, halfDir), 0.0), shininess);
	vec3 specularColor = lightSpe * objColor * specF;
	return ambientColor + isInShadow()*(diffuseColor + specularColor);
}

vec3 calDirectionalLight() {
	lightAmb = vec3(0.3, 0.3, 0.3);
	lightDiff = vec3(1.5, 1.5, 1.5);
	lightSpe = vec3(1.0, 1.0, 1.0);
	shininess = 32.0;
	return phong_shading();
}





void main() {
	UV = in_data.UV;
	normal = normalize(in_data.N);
	Pos = in_data.Pos;
	objColor = vec3(texture(texture_diffuse1, UV));
	out_color = vec4(calDirectionalLight(), 1.0);
}