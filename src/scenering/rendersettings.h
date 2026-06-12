#pragma once

#include "skybox.h"

class RenderSettings
{
public:
    RenderSettings() = default;
    ~RenderSettings() = default;

    inline bool getSkyboxEnabled() const { return m_skyboxEnabled; }
    inline void setSkyboxEnabled(bool skyboxEnabled) { m_skyboxEnabled = skyboxEnabled; }

    inline const Skybox& getSkybox() const { return m_skybox; }
    inline void setSkybox(const Skybox& skybox) { m_skybox = skybox; }

    inline float getShadowDistance() const { return m_shadowDistance; }
    inline void setShadowDistance(float shadowDistance) { m_shadowDistance = shadowDistance; }

    inline float getShadowBias() const { return m_shadowBias; }
    inline void setShadowBias(float shadowBias) { m_shadowBias = shadowBias; }

private:
    bool m_skyboxEnabled = true;
    Skybox m_skybox;
    float m_shadowDistance = 100.0f;
    float m_shadowBias = 0.005f;
};