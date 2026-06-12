#include "scenemanager.h"

#include <stdexcept>

void SceneManager::loadScene(std::unique_ptr<ISceneSetup> setup)
{
    unload();
    m_activeSetup = std::move(setup);
    m_scene = std::make_unique<Scene>(m_activeSetup->createRenderSettings());
    m_activeSetup->build(*m_scene);
    m_scene->start();
}

void SceneManager::unload()
{
    m_scene.reset();
    m_activeSetup.reset();
}

void SceneManager::update(float deltaTime)
{
    if (!m_scene)
        return;

    m_scene->update(deltaTime);
}

Scene& SceneManager::getActiveScene()
{
    if (!m_scene)
        throw std::runtime_error("No active scene loaded.");

    return *m_scene;
}

bool SceneManager::hasActiveScene() const
{
    return m_scene != nullptr;
}
