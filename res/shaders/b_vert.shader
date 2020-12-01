#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 aColor;
out vec4 color;
uniform vec4 u_Color;

void main()
{
    gl_Position = vec4(pos.x, -pos.y, pos.z, 1.0);
    color = u_Color;
};