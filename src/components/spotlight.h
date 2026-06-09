#pragma once

#include "light.h"

class Spotlight : public Light
{
public:
    Spotlight(
        GameObject* owner,
        const glm::vec3& color,
        float intensity,
        float constant,
        float linear,
        float quadratic,
        float innerCutoff,
        float outerCutoff
    )
        : Light(owner, color, intensity), m_constant(constant), m_linear(linear), m_quadratic(quadratic), m_innerCutoff(innerCutoff), m_outerCutoff(outerCutoff) {}
    ~Spotlight() = default;

    float getConstant() const { return m_constant; }
    void setConstant(float constant) { m_constant = constant; }

    float getLinear() const { return m_linear; }
    void setLinear(float linear) { m_linear = linear; }

    float getQuadratic() const { return m_quadratic; }
    void setQuadratic(float quadratic) { m_quadratic = quadratic; }

    float getInnerCutoff() const { return m_innerCutoff; }
    void setInnerCutoff(float innerCutoff) { m_innerCutoff = innerCutoff; }

    float getOuterCutoff() const { return m_outerCutoff; }
    void setOuterCutoff(float outerCutoff) { m_outerCutoff = outerCutoff; }

private:
    float m_constant;
    float m_linear;
    float m_quadratic;
    float m_innerCutoff;
    float m_outerCutoff;
};