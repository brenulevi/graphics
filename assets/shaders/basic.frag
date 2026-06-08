#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// Directional light structure
struct Light
{
    vec3 direction;
    vec3 color;
    float intensity;
};

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPos;

uniform Material u_Material;
uniform Light u_Light;

uniform vec3 u_ViewPos;

out vec4 o_FragColor;

void main()
{
    vec3 norm = normalize(v_Normal);

    vec3 lightDir = normalize(-u_Light.direction);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_Light.color * u_Light.intensity;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_Light.color * u_Light.intensity;

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = spec * u_Light.color * u_Light.intensity;

    // Texture sampling
    vec3 diffuseTex = texture(u_Material.diffuse, v_TexCoord).rgb;
    vec3 specularTex = texture(u_Material.specular, v_TexCoord).rgb;

    // Combine results
    vec3 result = (ambient * diffuseTex + diffuse * diffuseTex + specular * specularTex);
    o_FragColor = vec4(result, 1.0);
}
