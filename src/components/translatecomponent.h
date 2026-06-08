#pragma once

#include "component.h"
#include "scenering/gameobject.h"
#include "transform.h"

class TranslateComponent : public Component
{
public:
    TranslateComponent(GameObject* owner, float speed);
    ~TranslateComponent() = default;

    void start() override;
    void update(float deltaTime) override;

private:
    float m_speed;
    float m_totalTime = 0.0f;
};