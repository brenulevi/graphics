#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "component.h"

class Transform : public Component
{
public:
    Transform(
        GameObject* owner,
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& rotation = glm::vec3(0.0f),
        const glm::vec3& scale = glm::vec3(1.0f)
    );
    ~Transform() = default;

    inline glm::vec3 getPosition() { return m_position; }
    inline void setPosition(const glm::vec3& pos) { m_position = pos; }

    inline glm::vec3 getRotation() { return m_rotation; }
    inline void setRotation(const glm::vec3& rot) { m_rotation = rot; }

    inline glm::vec3 getScale() { return m_scale; }
    inline void setScale(const glm::vec3& s) { m_scale = s; }

    inline void translate(const glm::vec3& delta) { m_position += delta; }
    inline void rotate(const glm::vec3& delta) { m_rotation += delta; }
    inline void scale(const glm::vec3& delta) { m_scale += delta; }

    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;

    glm::mat4 getModelMatrix() const;
    glm::mat4 getViewMatrix() const;

private:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};