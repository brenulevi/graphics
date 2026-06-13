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

        m_imguiLayer->beginFrame();

        if (Input::isMouseLookActive())
        {
            ImGuiIO& blockedIo = ImGui::GetIO();
            for (int i = 0; i < IM_ARRAYSIZE(blockedIo.MouseDown); ++i)
                blockedIo.MouseDown[i] = false;
            blockedIo.MouseWheel = 0.0f;
            blockedIo.MouseWheelH = 0.0f;
        }

        m_sceneEditor.drawDockspace();

        if (Input::isMouseLookActive())
            ImGui::BeginDisabled(true);

        m_sceneEditor.drawPanels(m_sceneManager.getActiveScene());

        if (Input::isMouseLookActive())
            ImGui::EndDisabled();

        GameViewState gameViewState = m_gameView.begin();
        if (gameViewState.isOpen && gameViewState.width > 0 && gameViewState.height > 0)
        {
            m_gameView.bind();
            m_renderer->setViewportSize(static_cast<float>(gameViewState.width),
                                        static_cast<float>(gameViewState.height));
            m_renderer->clear();
            m_renderSystem->render(m_sceneManager.getActiveScene(), *m_renderer);
            m_gameView.unbind();
        }
        m_gameView.end();

        const bool gameHasPriority = m_gameView.isActive() || Input::isGameMode();
        Input::setGameViewFocused(gameHasPriority);

        ImGuiIO& io = ImGui::GetIO();
        Input::setUiCapturing((io.WantCaptureKeyboard || io.WantCaptureMouse) && !gameHasPriority);

        m_sceneManager.update(deltaTime);

        // NoMouseCursorChange is set — GLFW owns the hardware cursor; never draw ImGui's software cursor.
        io.MouseDrawCursor = false;

        if (Input::isMouseLookActive())
            Input::setCursorMode(GLFW_CURSOR_DISABLED);

        m_imguiLayer->endFrame(m_window->getWidth(), m_window->getHeight());

        m_window->swapBuffers();
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
