#pragma once

#include <stdexcept>

#include "component.h"
#include "scenering/gameobject.h"
#include "transform.h"
#include "input.h"

class PlayerController : public Component
{
public:
    PlayerController(GameObject* owner, float moveSpeed, float sensitivity);
    ~PlayerController() = default;

    void start() override;
    void update(float deltaTime) override;

    float getMoveSpeed() const { return m_moveSpeed; }
    void setMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }

    float getSensitivity() const { return m_sensitivity; }
    void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }

private:
    float m_moveSpeed;
    float m_sensitivity;
    bool m_firstMouse = true;
    float m_yaw = -90.0f;
    float m_pitch = 0.0f;
};