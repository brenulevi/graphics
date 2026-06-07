#include "rotationComponent.h"

RotationComponent::RotationComponent(GameObject *owner, float speed)
    : Component(owner), m_speed(speed)
{
}

void RotationComponent::start()
{
}

void RotationComponent::update(float deltaTime)
{
    auto transform = getOwner()->getComponent<Transform>();
    if (!transform)
        throw std::runtime_error("RotationComponent requires a Transform component.");

    transform->rotate(glm::vec3(0.5f, 1.0f, 0.0f) * m_speed * deltaTime);
}
