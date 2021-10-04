//#version 330 core
//
//out vec4 fragPos;
//layout(triangles) in;
//layout(triangle_strip, max_vertices=18) out;
//
//uniform mat4 shadowMatrix[6];
//void main() {
//	// for each face we calculate the clip space
//	// of this vertex as well as outputing
//	// the world position
//
//	// if the vertex is within one of perspective
//	// frustum, then it won't get culled and would
//	// be passed down to frag shader
//	for (int face = 0; face < 6; face++) {
//		gl_Layer = face;
//		for (int i = 0; i < 3; i++) {
//			fragPos = gl_in[i].gl_Position;
//			gl_Position = shadowMatrix[face] * fragPos;
//			EmitVertex();
//		}
//		EndPrimitive();
//	}
//}

#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = face; // built-in variable that specifies to which face we render.
		for (int i = 0; i < 3; ++i) // for each triangle's vertices
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}