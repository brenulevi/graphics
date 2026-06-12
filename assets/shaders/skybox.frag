#version 330 core

in vec3 v_TexCoord;

uniform samplerCube u_Cubemap;

out vec4 o_FragColor;

void main()
{
    o_FragColor = texture(u_Cubemap, v_TexCoord);
}