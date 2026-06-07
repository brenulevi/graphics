#version 330 core

in vec3 v_Normal;
in vec2 v_TexCoord;

uniform vec3 u_Color;

out vec4 o_FragColor;

void main()
{
    o_FragColor = vec4(u_Color, 1.0);
}
