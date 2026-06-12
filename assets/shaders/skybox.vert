#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_TexCoord;

uniform mat4 u_ViewProjection;

out vec3 v_TexCoord;

void main()
{
    v_TexCoord = i_Pos;
    vec4 position = u_ViewProjection * vec4(i_Pos, 1.0);
    gl_Position = position.xyww;
}