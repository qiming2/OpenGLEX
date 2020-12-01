#version 330 core

out vec4 out_color;
in vec4 color;
uniform vec4 u_Color;

void main()
{
    out_color = u_Color;
};