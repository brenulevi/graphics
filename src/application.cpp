#include "application.h"

Application::Application()
{
    m_window = std::make_unique<Window>(800, 600, "My Game");
    m_window->setCloseCallback(std::bind(&Application::onClose, this));
    m_window->setResizeCallback(std::bind(&Application::onResize, this, std::placeholders::_1, std::placeholders::_2));

    m_renderer = std::make_unique<Renderer>();
    m_renderer->setViewportSize(m_window->getWidth(), m_window->getHeight());

    m_renderSystem = std::make_unique<RenderSystem>();

    RenderSettings renderSettings;
    m_scene = std::make_unique<Scene>(renderSettings);

    AssetManager::loadShader("default", "assets/shaders/basic.vert", "assets/shaders/basic.frag");
    
    auto containerDiffuse = AssetManager::loadTexture("container_diffuse", "assets/textures/container.png");
    containerDiffuse->setWrapMode(TextureWrapMode::Repeat, TextureWrapMode::Repeat);
    containerDiffuse->setFilterMode(TextureFilterMode::Linear, TextureFilterMode::Linear);
    
    AssetManager::loadTexture("container_specular", "assets/textures/container_specular.png");
    
    std::shared_ptr<Material> material = std::make_shared<Material>(
        AssetManager::getShader("default"),
        AssetManager::getTexture("container_diffuse"),
        AssetManager::getTexture("container_specular"),
        32.0f);

    auto floorDiffuse = AssetManager::loadTexture("floor_diffuse", "assets/textures/floor.jpg");
    floorDiffuse->setWrapMode(TextureWrapMode::Repeat, TextureWrapMode::Repeat);
    floorDiffuse->setFilterMode(TextureFilterMode::Linear, TextureFilterMode::Linear);
    
    auto floorSpecular = AssetManager::loadTexture("floor_specular", "assets/textures/floor_specular.jpg");
    floorSpecular->setWrapMode(TextureWrapMode::Repeat, TextureWrapMode::Repeat);
    floorSpecular->setFilterMode(TextureFilterMode::Linear, TextureFilterMode::Linear);
    
    std::shared_ptr<Material> floorMaterial = std::make_shared<Material>(
        AssetManager::getShader("default"),
        AssetManager::getTexture("floor_diffuse"),
        AssetManager::getTexture("floor_specular"),
        32.0f);

    std::vector<Vertex> quadVertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    };
    std::vector<unsigned int> quadIndices = {
        0, 1, 2, 2, 3, 0,
    };
    std::shared_ptr<Mesh> quadMesh = std::make_shared<Mesh>(quadVertices, quadIndices);

    std::vector<Vertex> cubeVertices = {
        // Front face (z =  0.5)
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

        // Back face (z = -0.5)
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

        // Left face (x = -0.5)
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // Right face (x = 0.5)
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // Top face (y = 0.5)
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

        // Bottom face (y = -0.5)
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}};
    std::vector<unsigned int> cubeIndices = {
        0, 1, 2, 2, 3, 0,       // Front
        4, 5, 6, 6, 7, 4,       // Back
        8, 9, 10, 10, 11, 8,    // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Top
        20, 21, 22, 22, 23, 20  // Bottom
    };
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(cubeVertices, cubeIndices);

    auto cube = m_scene->createGameObject("Cube");
    cube->addComponent<Transform>();
    cube->addComponent<MeshRenderer>(mesh, material);

    auto plane = m_scene->createGameObject("Plane");
    auto planeTransform = plane->addComponent<Transform>();
    plane->addComponent<MeshRenderer>(quadMesh, floorMaterial);
    planeTransform->setLocalPosition(glm::vec3(0.0f, -2.0f, 0.0f));
    planeTransform->setLocalRotationEuler(glm::vec3(-90.0f, 0.0f, 0.0f));
    planeTransform->setLocalScale(glm::vec3(10.0f, 10.0f, 1.0f));

    auto player = m_scene->createGameObject("Player");
    auto playerTransform = player->addComponent<Transform>(glm::vec3(0.0f, 0.0f, 2.0f));
    // playerTransform->setLocalRotationEuler(glm::vec3(0.0f, -90.0f, 0.0f));
    auto camera = player->addComponent<Camera>();
    player->addComponent<PlayerController>(5.0f, 0.1f);
    // player->addComponent<Spotlight>(glm::vec3(1.0f), 1.0f, 1.0f, 0.14f, 0.07f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
    m_scene->setMainCamera(camera);

    auto sun = m_scene->createGameObject("Sun");
    auto lightTransform = sun->addComponent<Transform>(glm::vec3(0.0f, 5.0f, 0.0f));
    lightTransform->setLocalRotationEuler(glm::vec3(-70.0f, 0.0f, 0.0f));
    sun->addComponent<DirectionalLight>(glm::vec3(1.0f), 1.0f);

    auto pointLight = m_scene->createGameObject("PointLight");
    auto pointLightTransform = pointLight->addComponent<Transform>(glm::vec3(2.0f, 1.0f, 0.0f));
    pointLightTransform->setLocalScale(glm::vec3(0.2f));
    pointLight->addComponent<PointLight>(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f, 0.14f, 0.07f);
    pointLight->addComponent<MeshRenderer>(mesh, material);

    auto backpack = m_scene->instantiateModel(
        AssetManager::loadModel("backpack", "assets/models/backpack/backpack.obj")
    );
    auto backpackTransform = backpack->getComponent<Transform>();
    backpackTransform->setLocalPosition(glm::vec3(-2.5f, 5.0f, 0.0f));

    m_scene->start();
}

Application::~Application()
{
    m_scene.reset();
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

        auto sun = m_scene->getGameObjectByName("Sun");
        auto sunTransform = sun->getComponent<Transform>();
        sunTransform->rotateEuler(glm::vec3(-1.0f, 0.0f, 0.0f) * deltaTime);

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
