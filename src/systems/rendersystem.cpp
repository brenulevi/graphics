#include "rendersystem.h"

void RenderSystem::render(Scene &scene, Renderer &renderer)
{
    Camera *mainCamera = scene.getMainCamera();
    if (!mainCamera)
    {
        throw std::runtime_error("No main camera set in the scene.");
    }

    renderer.beginScene(*mainCamera->getOwner()->getComponent<Transform>(), *mainCamera);

    auto meshRenderers = scene.getComponents<MeshRenderer>();
    for (MeshRenderer *meshRenderer : meshRenderers)
    {
        auto *transform = meshRenderer->getOwner()->getComponent<Transform>();
        renderer.draw(*transform, *meshRenderer->getMesh(), *meshRenderer->getMaterial());
    }

    renderer.endScene();
}