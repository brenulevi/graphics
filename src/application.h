#pragma once

#include <functional>
#include <memory>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "input.h"
#include "window.h"
#include "rendering/renderer.h"
#include "resources/assetmanager.h"
#include "scenes/demoscene.h"
#include "systems/rendersystem.h"
#include "scenering/scenemanager.h"
#include "editor/imguilayer.h"
#include "editor/sceneeditor.h"
#include "editor/gameview.h"
#include "editor/playmode.h"

class Application
{
public:
    Application();
    ~Application();

    void run();

private:
    void onClose();
    void onResize(int width, int height);
    void updateEditorInput(bool gameViewActive);

private:
    bool m_isRunning = true;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<RenderSystem> m_renderSystem;
    std::unique_ptr<ImGuiLayer> m_imguiLayer;
    SceneEditor m_sceneEditor;
    GameView m_gameView;
    PlayMode m_playMode;
    SceneManager m_sceneManager;
};
