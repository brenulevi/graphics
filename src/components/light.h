#pragma once

#include <glm/glm.hpp>

#include "component.h"

class Light : public Component
{
public:
    Light(GameObject* owner, const glm::vec3& color, float intensity)
        : Component(owner), m_color(color), m_intensity(intensity) {}
    ~Light() = default;

    glm::vec3 getColor() const { return m_color; }
    float getIntensity() const { return m_intensity; }

    void setColor(const glm::vec3& color) { m_color = color; }
    void setIntensity(float intensity) { m_intensity = intensity; }

private:
    glm::vec3 m_color;
    float m_intensity;
};