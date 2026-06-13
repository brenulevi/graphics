#include "demoscene.h"

RenderSettings DemoScene::createRenderSettings()
{
    RenderSettings settings;
    settings.setSkybox(Skybox(AssetManager::loadCubemap("skybox", {
        "assets/textures/skybox/right.jpg",
        "assets/textures/skybox/left.jpg",
        "assets/textures/skybox/top.jpg",
        "assets/textures/skybox/bottom.jpg",
        "assets/textures/skybox/front.jpg",
        "assets/textures/skybox/back.jpg"
    })));
    return settings;
}

void DemoScene::build(Scene& scene)
{
    AssetManager::loadShader("default", "assets/shaders/basic.vert",
                             "assets/shaders/basic.frag");
    AssetManager::loadShader("unlit", "assets/shaders/unlit.vert",
                             "assets/shaders/unlit.frag");

    auto containerDiffuse = AssetManager::loadTexture(
        "container_diffuse", "assets/textures/container.png");
    containerDiffuse->setWrapMode(TextureWrapMode::Repeat,
                                  TextureWrapMode::Repeat);
    containerDiffuse->setFilterMode(TextureFilterMode::Linear,
                                    TextureFilterMode::Linear);

    AssetManager::loadTexture("container_specular",
                              "assets/textures/container_specular.png");

    AssetManager::registerMaterial(
        "container",
        Material::createStandard(
            AssetManager::getShader("default"),
            AssetManager::getTexture("container_diffuse"),
            AssetManager::getTexture("container_specular"), 32.0f));

    auto floorDiffuse =
        AssetManager::loadTexture("floor_diffuse", "assets/textures/floor.jpg");
    floorDiffuse->setWrapMode(TextureWrapMode::Repeat, TextureWrapMode::Repeat);
    floorDiffuse->setFilterMode(TextureFilterMode::Linear,
                                TextureFilterMode::Linear);

    auto floorSpecular = AssetManager::loadTexture(
        "floor_specular", "assets/textures/floor_specular.jpg");
    floorSpecular->setWrapMode(TextureWrapMode::Repeat, TextureWrapMode::Repeat);
    floorSpecular->setFilterMode(TextureFilterMode::Linear,
                                 TextureFilterMode::Linear);

    AssetManager::registerMaterial(
        "floor",
        Material::createStandard(
            AssetManager::getShader("default"),
            AssetManager::getTexture("floor_diffuse"),
            AssetManager::getTexture("floor_specular"), 32.0f));

    AssetManager::registerMaterial(
        "point_light",
        Material::createUnlit(
            AssetManager::getShader("unlit"),
            AssetManager::getTexture("container_diffuse"),
            glm::vec3(3.0f, 3.0f, 2.0f)));

    std::vector<Vertex> quadVertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    };
    std::vector<unsigned int> quadIndices = {
        0, 1, 2, 2, 3, 0,
    };
    std::shared_ptr<Mesh> quadMesh =
        std::make_shared<Mesh>(quadVertices, quadIndices);

    std::vector<Vertex> cubeVertices = {
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}};
    std::vector<unsigned int> cubeIndices = {
        0,  1,  2,  2,  3,  0,
        4,  5,  6,  6,  7,  4,
        8,  9,  10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    std::shared_ptr<Mesh> mesh =
        std::make_shared<Mesh>(cubeVertices, cubeIndices);

    auto cube = scene.createGameObject("Cube");
    cube->addComponent<Transform>();
    cube->addComponent<MeshRenderer>(mesh, AssetManager::getMaterial("container"));

    auto plane = scene.createGameObject("Plane");
    auto planeTransform = plane->addComponent<Transform>();
    plane->addComponent<MeshRenderer>(quadMesh, AssetManager::getMaterial("floor"));
    planeTransform->setLocalPosition(glm::vec3(0.0f, -2.0f, 0.0f));
    planeTransform->setLocalRotationEuler(glm::vec3(-90.0f, 0.0f, 0.0f));
    planeTransform->setLocalScale(glm::vec3(10.0f, 10.0f, 1.0f));

    auto player = scene.createGameObject("Player");
    player->addComponent<Transform>(glm::vec3(0.0f, 0.0f, 2.0f));
    auto camera = player->addComponent<Camera>();
    player->addComponent<PlayerController>(5.0f, 0.1f);
    scene.setMainCamera(camera);

    auto sun = scene.createGameObject("Sun");
    auto lightTransform =
        sun->addComponent<Transform>(glm::vec3(0.0f, 5.0f, 0.0f));
    lightTransform->setLocalRotationEuler(glm::vec3(-70.0f, 0.0f, 0.0f));
    sun->addComponent<DirectionalLight>(glm::vec3(1.0f), 1.0f);

    auto pointLight = scene.createGameObject("PointLight");
    auto pointLightTransform =
        pointLight->addComponent<Transform>(glm::vec3(2.0f, 1.0f, 0.0f));
    pointLightTransform->setLocalScale(glm::vec3(0.2f));
    pointLight->addComponent<PointLight>(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f,
                                         0.14f, 0.07f);
    pointLight->addComponent<MeshRenderer>(mesh, AssetManager::getMaterial("point_light"));

    auto backpack = scene.instantiateModel(AssetManager::loadModel(
        "backpack", "assets/models/backpack/backpack.obj"));
    auto backpackTransform = backpack->getComponent<Transform>();
    backpackTransform->setLocalPosition(glm::vec3(-2.5f, 5.0f, 0.0f));
}
