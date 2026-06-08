#pragma once

#include "light.h"

class PointLight : public Light
{
public:
    PointLight(
        GameObject* owner,
        const glm::vec3& color,
        float intensity,
        float constant,
        float linear,
        float quadratic
    )
        : Light(owner, color, intensity), m_constant(constant), m_linear(linear), m_quadratic(quadratic) {}
    ~PointLight() = default;

    float getConstant() const { return m_constant; }
    void setConstant(float constant) { m_constant = constant; }

    float getLinear() const { return m_linear; }
    void setLinear(float linear) { m_linear = linear; }

    float getQuadratic() const { return m_quadratic; }
    void setQuadratic(float quadratic) { m_quadratic = quadratic; }

private:
    float m_constant;
    float m_linear;
    float m_quadratic;
};