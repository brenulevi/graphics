#pragma once

#include "light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight(GameObject* owner, const glm::vec3& color, float intensity)
        : Light(owner, color, intensity) {}
    ~DirectionalLight() = default;
};