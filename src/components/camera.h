#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "component.h"
#include "transform.h"

class Camera : public Component
{
public:
    Camera(GameObject* owner, float fov = 60.0f, float nearPlane = 0.1f, float farPlane = 100.0f)
        : Component(owner), m_fov(fov), m_nearPlane(nearPlane), m_farPlane(farPlane) {}
    ~Camera() = default;

    inline float getFOV() const { return m_fov; }
    inline void setFOV(float fov) { m_fov = fov; }

    inline float getNearPlane() const { return m_nearPlane; }
    inline void setNearPlane(float nearPlane) { m_nearPlane = nearPlane; }

    inline float getFarPlane() const { return m_farPlane; }
    inline void setFarPlane(float farPlane) { m_farPlane = farPlane; }

    inline glm::mat4 getProjectionMatrix(float aspectRatio) const { return glm::perspective(glm::radians(m_fov), aspectRatio, m_nearPlane, m_farPlane); }
    inline glm::mat4 getViewMatrix() const
    {
        auto transform = getGameObject()->getComponent<Transform>();
        if (!transform)
            throw std::runtime_error("Camera has no transform component.");

        return glm::lookAt(transform->getWorldPosition(), transform->getWorldPosition() + transform->getForward(), transform->getUp());
    }

private:
    float m_fov;
    float m_nearPlane;
    float m_farPlane;
};