#version 330 core

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// Directional light structure
struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float intensity;
};

// Point light structure
struct PointLight
{
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

// Spot light structure
struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPos;

uniform Material u_Material;

uniform DirectionalLight u_DirectionalLight;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform SpotLight u_SpotLights[MAX_SPOT_LIGHTS];
uniform int u_NumPointLights;
uniform int u_NumSpotLights;

uniform vec3 u_ViewPos;

out vec4 o_FragColor;

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);

void main()
{
    vec3 norm = normalize(v_Normal);

    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    vec3 diffuseTex = texture(u_Material.diffuse, v_TexCoord).rgb;
    vec3 specularTex = texture(u_Material.specular, v_TexCoord).rgb;

    vec3 ambient = diffuseTex * 0.1f * u_DirectionalLight.color * u_DirectionalLight.intensity;

    vec3 result = ambient;

    result += calcDirectionalLight(u_DirectionalLight, norm, viewDir, diffuseTex, specularTex);

    for (int i = 0; i < u_NumPointLights; ++i)
    {
        result += calcPointLight(u_PointLights[i], norm, v_FragPos, viewDir, diffuseTex, specularTex);
    }

    for (int i = 0; i < u_NumSpotLights; ++i)
    {
        result += calcSpotLight(u_SpotLights[i], norm, v_FragPos, viewDir, diffuseTex, specularTex);
    }

    o_FragColor = vec4(result, 1.0f);  
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuseTex, vec3 specularTex)
{
    vec3 lightColor = light.color * light.intensity;
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    // Combine results
    vec3 diffuse = diff * lightColor * diffuseTex;
    vec3 specular = spec * lightColor * specularTex;

    return diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex)
{
    vec3 lightVec = light.position - fragPos;
    float distance = length(lightVec);
    vec3 lightDir = lightVec / max(distance, 0.0001);
    vec3 lightColor = light.color * light.intensity;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0f;
    if (diff > 0.0f)
    {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    }

    // Attenuation
    float attenuationDenominator = light.constant + light.linear * distance + light.quadratic * (distance * distance);
    float attenuation = 1.0 / max(attenuationDenominator, 0.0001);

    // Combine results
    vec3 diffuse = diff * lightColor * diffuseTex;
    vec3 specular = spec * lightColor * specularTex;

    return (diffuse + specular) * attenuation;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex)
{
    vec3 lightVec = light.position - fragPos;
    float distance = length(lightVec);
    vec3 lightDir = lightVec / max(distance, 0.0001);
    vec3 lightColor = light.color * light.intensity;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0f;
    if (diff > 0.0f)
    {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    }

    // Attenuation
    float attenuationDenominator = light.constant + light.linear * distance + light.quadratic * (distance * distance);
    float attenuation = 1.0 / max(attenuationDenominator, 0.0001);

    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / max(epsilon, 0.0001), 0.0, 1.0);

    // Combine results
    vec3 diffuse = diff * lightColor * diffuseTex;
    vec3 specular = spec * lightColor * specularTex;

    return (diffuse + specular) * attenuation * intensity;
}