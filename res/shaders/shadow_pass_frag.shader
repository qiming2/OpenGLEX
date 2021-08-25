#version 330 core

void main() {
	// this line is not needed if having specified to
	// write depth value to a texture
	gl_FragDepth = gl_FragCoord.z;
}