#pragma once

#include "light.h"
#include "rendering/shadowmap.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight(GameObject* owner, const glm::vec3& color, float intensity)
        : Light(owner, color, intensity), m_shadowMap(std::make_unique<ShadowMap>(2048, 2048)) {}
    ~DirectionalLight() = default;

    ShadowMap& getShadowMap() { return *m_shadowMap; }

private:
    std::unique_ptr<ShadowMap> m_shadowMap;
};