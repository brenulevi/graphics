#pragma once

#include <vector>
#include <memory>

#include "gameobject.h"
#include "rendersettings.h"
#include "components/camera.h"
#include "rendering/model.h"
#include "components/meshrenderer.h"
#include "components/transform.h"
#include "resources/assetmanager.h"

class Scene
{
public:
    Scene(const RenderSettings& renderSettings);
    ~Scene() = default;

    GameObject* createGameObject(const std::string& name = "GameObject");

    template <typename T>
    std::vector<T*> getComponents() const;

    void start();
    void update(float deltaTime);

    inline Camera* getMainCamera() const { return m_mainCamera; }
    inline void setMainCamera(Camera* camera) { m_mainCamera = camera; }

    inline const RenderSettings& getRenderSettings() const { return m_renderSettings; }
    inline RenderSettings& getRenderSettings() { return m_renderSettings; }

    GameObject* instantiateModel(const std::shared_ptr<Model>& model);

    GameObject* getGameObjectByName(const std::string& name) const;

    std::vector<GameObject*> getGameObjects() const;
    std::vector<GameObject*> getRootGameObjects() const;

private:
    GameObject* instantiateModelNode(const ModelNode& node, const std::shared_ptr<Model>& model, GameObject* parent);

private:
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    Camera* m_mainCamera = nullptr;
    RenderSettings m_renderSettings;
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
