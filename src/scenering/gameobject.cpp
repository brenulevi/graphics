#include "gameobject.h"

void GameObject::start()
{
    for (const auto &component : m_components)
    {
        component->start();
    }
}

void GameObject::update(float deltaTime)
{
    for (const auto &component : m_components)
    {
        component->update(deltaTime);
    }
}