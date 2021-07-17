#version 330 core

out vec4 out_color;
in vec3 Pos;
in vec3 Normal;

uniform vec3 viewPos;
uniform samplerCube skybox;

float glass = 1.52;
float air = 1.00;

// Currently, this is very basic and only render
// reflect and refract rays based on static skybox
void main() {
	vec3 viewDir = normalize(Pos - viewPos);
	vec3 Nr = normalize(Normal);
	//////////////////////////////// Reflect ////////////////
	// glsl has a built in reflect
	// vec3 reflecDir = reflect(viewDir, Nr);
	
	// reflectDir implementation

	vec3 reflecDir =  2 * Nr * max(dot(Nr, -viewDir), 0.0) + viewDir;


	/////////////////////////////// Refract ////////////////
	// glsl has a built in refract
	//vec3 refractDir = refract(viewDir, Nr, air / glass);
	
	// refract implementation
	// right now hardcoded refractive indices
	// assuming always looking from air to glass
	float ratio = air / glass;
	float theta = max(dot(-Nr, viewDir), 0.0);
	float refract_component = sqrt(1 - ratio * ratio * (1 - theta * theta));

	vec3 refractDir = ratio * viewDir + (ratio * theta - refract_component) * Nr;
	out_color = vec4(texture(skybox, refractDir).rgb, 1.0);
}