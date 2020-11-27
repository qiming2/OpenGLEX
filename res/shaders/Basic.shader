#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 aColor;
out vec3 out_color;

void main()
{
   gl_Position = vec4(pos.xyz, 1.0);
   out_color = aColor;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec3 out_color;
uniform vec4 u_Color;

void main()
{
    color = vec4(out_color, 1.0);
};