#include "application.h"

Application::Application()
{
    m_window = std::make_unique<Window>(800, 600, "My Game");
    m_window->setCloseCallback(std::bind(&Application::onClose, this));
    m_window->setResizeCallback(std::bind(&Application::onResize, this, std::placeholders::_1, std::placeholders::_2));

    m_renderer = std::make_unique<Renderer>();
    m_renderer->setViewportSize(m_window->getWidth(), m_window->getHeight());

    m_renderSystem = std::make_unique<RenderSystem>();

    m_scene = std::make_unique<Scene>();

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    std::shared_ptr<Material> material = std::make_shared<Material>(shader);

    std::vector<Vertex> vertices = {
        { { -0.5f, -0.5f, -0.5f }, { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
        { {  0.5f, -0.5f, -0.5f }, {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f } },
        { {  0.5f,  0.5f, -0.5f }, {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
        { { -0.5f,  0.5f, -0.5f }, { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f } },
        { { -0.5f, -0.5f,  0.5f }, { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },
        { {  0.5f, -0.5f,  0.5f }, {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
        { {  0.5f,  0.5f,  0.5f }, {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f } },
        { { -0.5f,  0.5f,  0.5f }, { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f } }
    };
    std::vector<unsigned int> indices = {
        4, 5, 6, 6, 7, 4,
        1, 0, 3, 3, 2, 1,
        0, 4, 7, 7, 3, 0,
        5, 1, 2, 2, 6, 5,
        3, 7, 6, 6, 2, 3,
        0, 1, 5, 5, 4, 0
    };
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices, indices);

    auto cube = m_scene->createGameObject();
    cube->addComponent<Transform>();
    cube->addComponent<MeshRenderer>(mesh, material);
    cube->addComponent<RotationComponent>();

    auto player = m_scene->createGameObject();
    auto playerTransform = player->addComponent<Transform>(glm::vec3(0.0f, 0.0f, 3.0f));
    playerTransform->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
    auto camera = player->addComponent<Camera>();
    m_scene->setMainCamera(camera);

    m_scene->start();
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

        m_scene->update(deltaTime);

        m_renderer->clear();

        m_renderSystem->render(*m_scene, *m_renderer);

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
