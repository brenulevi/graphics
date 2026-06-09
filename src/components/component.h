#pragma once

class GameObject;

class Component
{
friend class RenderSystem;
public:
    Component(GameObject* gameObject)
        : m_gameObject(gameObject)
    {
    }
    virtual ~Component() = default;

    virtual void start() {}
    virtual void update(float deltaTime) {}

protected:
    inline GameObject* getGameObject() const { return m_gameObject; }

private:
    GameObject* m_gameObject;
};