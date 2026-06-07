#pragma once

#include <vector>
#include <memory>

#include "gameobject.h"
#include "components/camera.h"

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    GameObject* createGameObject();

    template <typename T>
    std::vector<T*> getComponents() const;

    void start();
    void update(float deltaTime);

    inline Camera* getMainCamera() const { return m_mainCamera; }
    inline void setMainCamera(Camera* camera) { m_mainCamera = camera; }

private:
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    Camera* m_mainCamera = nullptr;
};

template <typename T>
inline std::vector<T *> Scene::getComponents() const
{
    std::vector<T*> components;
    for (const auto &gameObject : m_gameObjects)
    {
        T* component = gameObject->getComponent<T>();
        if (component)
        {
            components.push_back(component);
        }
    }
    return components;
}
