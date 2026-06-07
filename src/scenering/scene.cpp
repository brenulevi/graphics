#include "scene.h"

GameObject *Scene::createGameObject()
{
    auto gameObject = std::make_unique<GameObject>();
    GameObject *gameObjectPtr = gameObject.get();
    m_gameObjects.push_back(std::move(gameObject));
    return gameObjectPtr;
}

void Scene::start()
{
    for (const auto &gameObject : m_gameObjects)
    {
        gameObject->start();
    }
}

void Scene::update(float deltaTime)
{
    for (const auto &gameObject : m_gameObjects)
    {
        gameObject->update(deltaTime);
    }
}