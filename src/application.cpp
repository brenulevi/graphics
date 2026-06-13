#include "application.h"

Application::Application()
{
    m_window = std::make_unique<Window>(1280, 720, "Graphics Editor");
    m_window->setCloseCallback(std::bind(&Application::onClose, this));
    m_window->setResizeCallback(std::bind(&Application::onResize, this,
                                          std::placeholders::_1,
                                          std::placeholders::_2));

    m_renderer = std::make_unique<Renderer>();
    m_renderer->setViewportSize(m_window->getWidth(), m_window->getHeight());

    m_renderSystem = std::make_unique<RenderSystem>();
    m_imguiLayer = std::make_unique<ImGuiLayer>(*m_window);

    m_sceneManager.loadScene(std::make_unique<DemoScene>());
}

Application::~Application()
{
    m_playMode.stop();

    m_imguiLayer.reset();
    m_sceneManager.unload();
    AssetManager::clear();
}

void Application::run()
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (m_isRunning)
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        m_window->pollEvents();
        Input::update();

        ImGui::GetIO().MouseDrawCursor = false;

        m_imguiLayer->beginFrame();

        if (Input::isMouseLookActive())
        {
            ImGuiIO& blockedIo = ImGui::GetIO();
            for (int i = 0; i < IM_ARRAYSIZE(blockedIo.MouseDown); ++i)
                blockedIo.MouseDown[i] = false;
            blockedIo.MouseWheel = 0.0f;
            blockedIo.MouseWheelH = 0.0f;
        }

        Scene& scene = m_sceneManager.getActiveScene();

        m_sceneEditor.drawDockspace(scene, m_playMode);
        m_sceneEditor.drawPanels(scene);

        if (Input::isMouseLookActive())
            ImGui::EndDisabled();

        GameViewState gameViewState = m_gameView.begin();
        if (gameViewState.isOpen && gameViewState.width > 0 && gameViewState.height > 0)
        {
            m_gameView.bind();
            m_renderer->setViewportSize(static_cast<float>(gameViewState.width),
                                        static_cast<float>(gameViewState.height));
            m_renderer->clear();
            m_renderSystem->render(scene, *m_renderer);
            m_gameView.unbind();
        }
        m_gameView.end();

        m_playMode.handleInput();
        updateEditorInput(m_gameView.isActive());

        if (m_playMode.consumePlayStarted())
            scene.start();

        if (m_playMode.isPlaying())
            m_sceneManager.update(deltaTime);

        m_imguiLayer->endFrame(m_window->getWidth(), m_window->getHeight());

        m_window->swapBuffers();
    }
}

void Application::updateEditorInput(bool gameViewActive)
{
    ImGuiIO& io = ImGui::GetIO();
    const bool captureGameCursor = m_playMode.isPlaying() && gameViewActive;

    if (captureGameCursor)
    {
        Input::setCursorMode(GLFW_CURSOR_DISABLED);
        io.WantCaptureKeyboard = false;
        io.WantCaptureMouse = false;
    }
    else
    {
        Input::setCursorMode(GLFW_CURSOR_NORMAL);
    }
}

void Application::onClose()
{
    m_isRunning = false;
}

void Application::onResize(int width, int height)
{
    m_renderer->setViewportSize(static_cast<float>(width), static_cast<float>(height));
}
