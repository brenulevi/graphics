#pragma once

#include <stdexcept>

#include "component.h"
#include "scenering/gameobject.h"
#include "transform.h"

class RotationComponent : public Component
{
public:
    RotationComponent(GameObject* owner, float speed = 90.0f);
    ~RotationComponent() = default;

    void start() override;
    void update(float deltaTime) override;

private:
    float m_speed;
};
