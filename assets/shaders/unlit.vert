#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec2 v_TexCoord;

void main()
{
    vec4 worldPos = u_Model * vec4(i_Pos, 1.0);
    v_TexCoord = i_TexCoord;
    gl_Position = u_ViewProjection * worldPos;
}
