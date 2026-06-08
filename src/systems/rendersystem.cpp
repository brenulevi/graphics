#include "rendersystem.h"

void RenderSystem::render(Scene &scene, Renderer &renderer)
{
    Camera *mainCamera = scene.getMainCamera();
    if (!mainCamera)
    {
        throw std::runtime_error("No main camera set in the scene.");
    }

    auto cameraTransform = mainCamera->getOwner()->getComponent<Transform>();
    if (!cameraTransform)
        throw std::runtime_error("Main camera has no transform component.");

    SceneData sceneData{
        .viewProjectionMatrix = mainCamera->getProjectionMatrix(renderer.getAspectRatio()) * cameraTransform->getViewMatrix(),
        .cameraPosition = cameraTransform->getPosition(),
        .lightDirection = glm::vec3(0.0f, -1.0f, 0.0f),
        .lightColor = glm::vec3(1.0f),
        .lightIntensity = 0.0f
    };

    auto directionalLight = scene.getComponents<DirectionalLight>();
    if (!directionalLight.empty())
    {
        sceneData.lightDirection = directionalLight[0]->getOwner()->getComponent<Transform>()->getForward();
        sceneData.lightColor = directionalLight[0]->getColor();
        sceneData.lightIntensity = directionalLight[0]->getIntensity();
    }

    renderer.beginScene(sceneData);

    auto meshRenderers = scene.getComponents<MeshRenderer>();
    for (MeshRenderer *meshRenderer : meshRenderers)
    {
        auto *transform = meshRenderer->getOwner()->getComponent<Transform>();
        renderer.draw(*transform, *meshRenderer->getMesh(), *meshRenderer->getMaterial());
    }

    renderer.endScene();
}