#pragma once

#include <memory>
#include <stdexcept>

#include "iscenesetup.h"
#include "scene.h"

class SceneManager
{
public:
    void loadScene(std::unique_ptr<ISceneSetup> setup);
    void unload();
    void update(float deltaTime);

    Scene& getActiveScene();
    bool hasActiveScene() const;

private:
    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<ISceneSetup> m_activeSetup;
};
