#version 330 core

out vec4 out_color;
in VS_OUT{
	vec3 Pos;
	vec3 Normal;
	vec2 UV;
	mat3 TBN;
} vs;

struct Light {
	vec3 intensity;
	vec3 dir;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
Light light;
uniform vec3 viewPos;
uniform bool setNorm;
uniform bool parallexMapping;
uniform float heightScale;

vec2 textureCoordWithHeightMap();

vec2 textureCoordWithHeightMap(vec3 TBNViewDir) {
	// get height value off the height map and then
	// scale normalized view vector by the height value
	float height = texture(texture_height1, vs.UV).r;
	// dividing by z is to make p larger when looking at a steep angle
	// than when TBNViewDir is perpendicular to the surface
	vec2 p = TBNViewDir.xy / TBNViewDir.z * (height * heightScale);
	return vs.UV - p;
}

void main() {
	light.intensity = vec3(1.0, 1.0, 1.0);
	light.dir = -normalize(vec3(0.0, 1.0, 1.0));
	vec3 viewDir = viewPos - vs.Pos;
	vec2 texCoord;
	if (parallexMapping) {
		vec3 TBNViewDir = normalize(transpose(vs.TBN) * viewDir);
		texCoord = textureCoordWithHeightMap(TBNViewDir);
		// if texture coordinates are out of bound, we discard the pixel
		if (texCoord.x > 1.0 || texCoord.x < 0.0 || texCoord.y > 1.0 || texCoord.y < 0.0)
			discard;
	}
	else {
		texCoord = vs.UV;
	}

	vec3 obj_color = texture(texture_diffuse1, texCoord).rgb;
	// transform sampled normal from tangent space to world space
	// since it is from 0 to 1, we want to map them back to -1 to 1
	vec3 normal = texture(texture_normal1, texCoord).rgb;
	normal = normalize(vs.TBN * (normal * 2.0 - 1.0));
	if (setNorm) {
		normal = vs.Normal;
	}
	
	
	// normal light calculation
	vec3 ambient = vec3(0.2, 0.2, 0.2) * light.intensity;
	float dotN = max(dot(normal, -light.dir), 0.0);
	vec3 diffuse = dotN * light.intensity;

	vec3 halfDir = normalize(-light.dir + viewDir);
	dotN = max(dot(halfDir, normal), 0.0);
	vec3 specular = pow(dotN, 32) * light.intensity;

	out_color = vec4((ambient + diffuse + specular) * obj_color, 1.0);
	//out_color = vec4(normal, 1.0);
}