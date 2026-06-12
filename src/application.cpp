#include "application.h"

#include "input.h"
#include "resources/assetmanager.h"
#include "scenes/demoscene.h"

Application::Application()
{
    m_window = std::make_unique<Window>(800, 600, "My Game");
    m_window->setCloseCallback(std::bind(&Application::onClose, this));
    m_window->setResizeCallback(std::bind(&Application::onResize, this,
                                          std::placeholders::_1,
                                          std::placeholders::_2));

    m_renderer = std::make_unique<Renderer>();
    m_renderer->setViewportSize(m_window->getWidth(), m_window->getHeight());

    m_renderSystem = std::make_unique<RenderSystem>();

    m_sceneManager.loadScene(std::make_unique<DemoScene>());
}

Application::~Application()
{
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

        m_sceneManager.update(deltaTime);

        m_renderer->clear();
        m_renderSystem->render(m_sceneManager.getActiveScene(), *m_renderer);
        m_window->swapBuffers();
    }
}

void Application::onClose()
{
    m_isRunning = false;
}

void Application::onResize(int width, int height)
{
    m_renderer->setViewportSize(width, height);
}
