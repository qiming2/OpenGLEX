#version 330 core

out vec4 out_color;
in vec3 color;
in vec2 UV;
uniform vec4 u_Color;
uniform vec4 a_Color;
uniform float mixVal;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
    vec2 uv = UV;
    // float alpha = 0.5 - abs(uv.x - 0.5) - abs(uv.y - 0.5);
    float color1 = texture(texture1, uv).r;
    vec3 color2 = texture(texture2, uv).rgb;

    vec3 color_temp =vec3(color1);
    out_color = vec4(color_temp, 1.0);
 
}
 