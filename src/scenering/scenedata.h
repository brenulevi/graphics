#pragma once

#include <glm/glm.hpp>
#include <vector>

struct DirectionalLightData
{
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};

struct PointLightData
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLightData
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;

    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};

struct SceneData
{
    glm::mat4 viewProjectionMatrix;
    
    glm::vec3 cameraPosition;
    
    DirectionalLightData directionalLight;
    std::vector<PointLightData> pointLights;
    std::vector<SpotLightData> spotLights;
};