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

void GameObject::setParent(GameObject *parent)
{
    if (m_parent == parent)
        return;

    if (parent == this)
    {
        throw std::runtime_error("A GameObject cannot be its own parent.");
    }

    if (parent && parent->isChildOf(this))
    {
        throw std::runtime_error("Cannot set parent: it would create a cycle in the hierarchy.");
    }

    if (m_parent)
    {
        auto &siblings = m_parent->m_children;
        siblings.erase(
            std::remove(
                siblings.begin(),
                siblings.end(),
                this
            ),
            siblings.end()
        );
    }

    m_parent = parent;

    if (m_parent)
    {
        m_parent->m_children.push_back(this);
    }
}

bool GameObject::isChildOf(const GameObject *potentialParent) const
{
    const GameObject *current = this;
    while (current)
    {
        if (current == potentialParent)
            return true;
        current = current->getParent();
    }
    return false;
}