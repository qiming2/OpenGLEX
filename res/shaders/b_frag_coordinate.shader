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
    vec2 uv = UV;
    // float alpha = 0.5 - abs(uv.x - 0.5) - abs(uv.y - 0.5);
    vec4 color1 = texture(texture1, uv);
    vec4 color2 = texture(texture2, uv);
    out_color = mix(color1, color2, 0.5);
    //out_color = color1;
 
}
 