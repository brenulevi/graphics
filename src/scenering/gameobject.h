#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <string>

#include "components/component.h"

class Scene;

class GameObject
{
public:
    GameObject(Scene& scene) : m_scene(scene) {}
    ~GameObject() = default;

    template <typename T, typename... Args>
    T *addComponent(Args &&...args);

    template <typename T>
    T *getComponent();

    void start();
    void update(float deltaTime);

    std::string getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    void setParent(GameObject* parent);
    bool isChildOf(const GameObject* potentialParent) const;
    inline GameObject* getParent() const { return m_parent; }
    inline const std::vector<GameObject*>& getChildren() const { return m_children; }

private:
    std::string m_name;
    std::vector<std::unique_ptr<Component>> m_components;
    Scene& m_scene;
    GameObject* m_parent = nullptr;
    std::vector<GameObject*> m_children;
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
