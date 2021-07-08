#version 330 core

in vec3 Pos;
in vec3 Normal;
in vec2 UV;


uniform vec3 outline_color;
out vec4 out_color;
out float gl_FragDepth;

void main() {
	// make outline always appear 
	// range from 0.0 to 1.0
	
	gl_FragDepth = gl_FragCoord.z - 2.0;
	out_color = vec4(outline_color.rgb, 1.0);
	//out_color = vec4(vec3(gl_FragCoord.z), 1.0);
	//out_color = vec4(1.0);
}