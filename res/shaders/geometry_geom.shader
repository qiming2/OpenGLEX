#version 330 core

////////////////////////// simple pass through geometry shader test//////////////////
//layout (points) in;
//layout (points, max_vertices = 1) out;
//
//void main() {    
//    gl_Position = gl_in[0].gl_Position; 
//    gl_PointSize = 10;
//    EmitVertex();
//    EndPrimitive();
//}  



//////////////////////////// line strip ///////////////////////
//layout (points) in;
//layout (line_strip, max_vertices = 2) out;
//
//
//void main() {
//	gl_Position = gl_in[0].gl_Position + vec4(-0.2, 0.0, 0.0, 0.0);
//	EmitVertex();
//	gl_Position = gl_in[0].gl_Position + vec4(0.2, 0.0, 0.0, 0.0);
//	EmitVertex();
//
//	EndPrimitive();
//}

/////////////////////////// triangle strip with different color /////////////////////
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;
vec4 gPos[5] = vec4[5](vec4(-0.2, -0.4, 0.0, 0.0), vec4(0.2, -0.4, 0.0, 0.0),
				vec4(-0.2, 0.0, 0.0, 0.0), vec4(0.2, 0.0, 0.0, 0.0), 
				vec4(0.0, 0.2, 0.0, 0.0));

in VS_OUT {
	vec3 Color;
} gs_in[];

out vec3 color;

void main() {
	gl_Position = gl_in[0].gl_Position + gPos[0];
	color = gs_in[0].Color;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + gPos[1];
	color = gs_in[0].Color;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + gPos[2];
	color = gs_in[0].Color;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + gPos[3];
	color = gs_in[0].Color;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + gPos[4];
	color = vec3(1.0, 1.0, 1.0);
	EmitVertex();
	EndPrimitive();
}