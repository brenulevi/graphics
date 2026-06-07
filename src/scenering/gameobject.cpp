#include "gameobject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

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