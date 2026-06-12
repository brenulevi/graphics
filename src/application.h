#pragma once

#include <memory>

#include "window.h"
#include "rendering/renderer.h"
#include "systems/rendersystem.h"
#include "scenering/scenemanager.h"

class Application
{
public:
    Application();
    ~Application();

    void run();

private:
    void onClose();
    void onResize(int width, int height);

private:
    bool m_isRunning = true;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<RenderSystem> m_renderSystem;
    SceneManager m_sceneManager;
};
