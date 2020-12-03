#version 330 core

out vec4 out_color;
in vec3 color;
in vec2 UV;
uniform vec4 u_Color;
uniform vec4 a_Color;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
    vec4 color1 = texture(texture1, UV);
    vec4 color2 = texture(texture2, UV);
    out_color = mix(color1, color2, 0.2);
}
 