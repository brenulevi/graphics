#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_ColorMap;
uniform vec3 u_Tint;

out vec4 o_FragColor;

void main()
{
    o_FragColor = vec4(texture(u_ColorMap, v_TexCoord).rgb * u_Tint, 1.0);
}
