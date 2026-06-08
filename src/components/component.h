#pragma once

class GameObject;

class Component
{
friend class RenderSystem;
public:
    Component(GameObject* owner)
        : m_owner(owner)
    {
    }
    virtual ~Component() = default;

    virtual void start() {}
    virtual void update(float deltaTime) {}

protected:
    inline GameObject* getOwner() const { return m_owner; }

private:
    GameObject* m_owner;
};