#pragma once

class GameObject;

class Component
{
public:
    Component(GameObject* owner)
        : m_owner(owner)
    {
    }
    virtual ~Component() = default;

    virtual void start() {}
    virtual void update(float deltaTime) {}

    inline GameObject* getOwner() const { return m_owner; }

private:
    GameObject* m_owner;
};