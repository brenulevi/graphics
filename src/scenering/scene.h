#pragma once

#include <vector>
#include <memory>

#include "gameobject.h"

class Scene
{
public:
    Scene();
    ~Scene();

    GameObject* createGameObject();

    void start();
    void update(float deltaTime);

private:
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};