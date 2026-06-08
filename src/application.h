#pragma once

#include <iostream>
#include <stdexcept>

#include "window.h"
#include "rendering/renderer.h"
#include "input.h"
#include "systems/rendersystem.h"
#include "scenering/scene.h"
#include "components/rotationcomponent.h"
#include "components/translatecomponent.h"
#include "components/directionallight.h"
#include "components/pointlight.h"
#include "components/playercontroller.h"

class Application
{
public:
    Application();
    ~Application() = default;

    void run();

private:
    void onClose();
    void onResize(int width, int height);

private:
    bool m_isRunning = true;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<RenderSystem> m_renderSystem;
    std::unique_ptr<Scene> m_scene;
};