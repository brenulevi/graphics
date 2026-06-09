#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "component.h"
#include "scenering/gameobject.h"

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

    inline glm::vec3 getLocalPosition() const { return m_localPosition; }
    inline void setLocalPosition(const glm::vec3& pos) { m_localPosition = pos; }

    inline glm::quat getLocalRotationQuat() const { return m_localRotation; }
    inline void setLocalRotationQuat(const glm::quat& rot) { m_localRotation = rot; }

    inline glm::vec3 getLocalRotationEuler() const { return glm::degrees(glm::eulerAngles(m_localRotation)); }
    inline void setLocalRotationEuler(const glm::vec3& rot) { m_localRotation = glm::quat(glm::radians(rot)); }

    inline glm::vec3 getLocalScale() const { return m_localScale; }
    inline void setLocalScale(const glm::vec3& s) { m_localScale = s; }

    glm::vec3 getWorldPosition() const;
    glm::quat getWorldRotation() const;
    glm::vec3 getWorldScale() const;

    inline void translate(const glm::vec3& delta) { m_localPosition += delta; }
    void rotateEuler(const glm::vec3& euler);
    inline void scaleBy(const glm::vec3& factor) { m_localScale *= factor; }

    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;

    glm::mat4 getLocalMatrix() const;
    glm::mat4 getWorldMatrix() const;

private:
    glm::vec3 m_localPosition;
    glm::quat m_localRotation;
    glm::vec3 m_localScale;
};