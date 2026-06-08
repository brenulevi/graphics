#include "translatecomponent.h"

TranslateComponent::TranslateComponent(GameObject *owner, float speed)
    : Component(owner), m_speed(speed)
{
}

void TranslateComponent::start()
{
}

void TranslateComponent::update(float deltaTime)
{
    m_totalTime += deltaTime;

    auto transform = getOwner()->getComponent<Transform>();
    if(transform)
    {
        transform->setPosition(glm::vec3(-sin(m_totalTime * m_speed), cos(m_totalTime * m_speed), 0.0f));
    }
}