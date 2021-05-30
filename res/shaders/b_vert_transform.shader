#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 texCoord;
out vec3 color;
out vec2 UV;
uniform vec4 u_Color;
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(pos.xyz, 1.0);
    color = aColor;
    UV = texCoord;
};