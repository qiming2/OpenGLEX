#version 330 core

in vec3 Pos;
in vec3 Normal;
in vec2 UV;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

out vec4 out_color;


uniform float far;
uniform float near;
 
// convert non-linear to linear depth
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

// non-linear is just a side effect of converting view space z to clip space z
void main() {
	float depth = (LinearizeDepth(gl_FragCoord.z) - near) / (far - near);
	out_color = vec4(vec3(depth), 1.0);
	
	//out_color = vec4(vec3(gl_FragCoord.z), 1.0);
	//out_color = vec4(1.0);
}