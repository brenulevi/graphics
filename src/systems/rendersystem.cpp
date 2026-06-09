#include "rendersystem.h"

void RenderSystem::render(Scene &scene, Renderer &renderer)
{
    Camera *mainCamera = scene.getMainCamera();
    if (!mainCamera)
    {
        throw std::runtime_error("No main camera set in the scene.");
    }

    auto cameraTransform = mainCamera->getGameObject()->getComponent<Transform>();
    if (!cameraTransform)
        throw std::runtime_error("Main camera has no transform component.");

    SceneData sceneData{
        .viewProjectionMatrix = mainCamera->getProjectionMatrix(renderer.getAspectRatio()) * mainCamera->getViewMatrix(),
        .cameraPosition = cameraTransform->getWorldPosition(),
        .directionalLight = {
            .direction = glm::vec3(0.0f, -1.0f, 0.0f),
            .color = glm::vec3(1.0f),
            .intensity = 0.0f
        }
    };

    auto directionalLight = scene.getComponents<DirectionalLight>();
    if (!directionalLight.empty())
    {
        auto directionalLightTransform = directionalLight[0]->getGameObject()->getComponent<Transform>();
        if (!directionalLightTransform)
            throw std::runtime_error("Directional light has no transform component.");

        sceneData.directionalLight.direction = directionalLightTransform->getForward();
        sceneData.directionalLight.color = directionalLight[0]->getColor();
        sceneData.directionalLight.intensity = directionalLight[0]->getIntensity();
    }

    auto pointLights = scene.getComponents<PointLight>();
    for (PointLight *pointLight : pointLights)
    {
        auto pointLightTransform = pointLight->getGameObject()->getComponent<Transform>();
        if (!pointLightTransform)
            throw std::runtime_error("Point light has no transform component.");

        PointLightData pointLightData{
            .position = pointLightTransform->getWorldPosition(),
            .color = pointLight->getColor(),
            .intensity = pointLight->getIntensity(),
            .constant = pointLight->getConstant(),
            .linear = pointLight->getLinear(),
            .quadratic = pointLight->getQuadratic()
        };
        sceneData.pointLights.push_back(pointLightData);
    }

    auto spotLights = scene.getComponents<Spotlight>();
    for (Spotlight *spotLight : spotLights)
    {
        auto spotLightTransform = spotLight->getGameObject()->getComponent<Transform>();
        if (!spotLightTransform)
            throw std::runtime_error("Spot light has no transform component.");

        SpotLightData spotLightData{
            .position = spotLightTransform->getWorldPosition(),
            .direction = spotLightTransform->getForward(),
            .color = spotLight->getColor(),
            .intensity = spotLight->getIntensity(),
            .constant = spotLight->getConstant(),
            .linear = spotLight->getLinear(),
            .quadratic = spotLight->getQuadratic(),
            .innerCutoff = spotLight->getInnerCutoff(),
            .outerCutoff = spotLight->getOuterCutoff()
        };
        sceneData.spotLights.push_back(spotLightData);
    }

    renderer.beginScene(sceneData);

    auto meshRenderers = scene.getComponents<MeshRenderer>();
    for (MeshRenderer *meshRenderer : meshRenderers)
    {
        auto *transform = meshRenderer->getGameObject()->getComponent<Transform>();
        renderer.draw(*transform, *meshRenderer->getMesh(), *meshRenderer->getMaterial());
    }

    renderer.endScene();
}