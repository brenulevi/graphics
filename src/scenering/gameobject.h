#pragma once

#include <vector>
#include <memory>

#include "components/component.h"

class GameObject
{
public:
    GameObject();
    ~GameObject();

    template <typename T, typename... Args>
    T *addComponent(Args &&...args);

    template <typename T>
    T *getComponent();

    void start();
    void update(float deltaTime);

private:
    std::vector<std::unique_ptr<Component>> m_components;
};

template <typename T, typename... Args>
inline T *GameObject::addComponent(Args &&...args)
{
    auto component = std::make_unique<T>(this, std::forward<Args>(args)...);

    T *componentPtr = component.get();
    m_components.push_back(std::move(component));

    return componentPtr;
}

template <typename T>
inline T *GameObject::getComponent()
{
    for (const auto &component : m_components)
    {
        if (auto casted = dynamic_cast<T *>(component.get()))
        {
            return casted;
        }
    }
    return nullptr;
}
