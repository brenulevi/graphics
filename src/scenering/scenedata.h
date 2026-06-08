#pragma once

#include <glm/glm.hpp>
#include <vector>

struct SceneData
{
    glm::mat4 viewProjectionMatrix;
    
    glm::vec3 cameraPosition;
    
    glm::vec3 lightDirection;
    glm::vec3 lightColor;
    float lightIntensity;
};