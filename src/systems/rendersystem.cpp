#include "rendersystem.h"

void RenderSystem::render(Scene &scene, Renderer &renderer)
{
    auto ctx = buildRenderContext(scene, renderer);

    setupDirectionalLight(ctx);
    renderShadowPass(ctx);

    collectPointLights(ctx);
    collectSpotLights(ctx);

    renderMainPass(ctx);
}

RenderSystem::RenderContext RenderSystem::buildRenderContext(Scene &scene, Renderer &renderer)
{
    auto renderSettings = scene.getRenderSettings();

    Camera *mainCamera = scene.getMainCamera();
    if (!mainCamera)
        throw std::runtime_error("No main camera set in the scene.");

    auto cameraTransform = mainCamera->getGameObject()->getComponent<Transform>();
    if (!cameraTransform)
        throw std::runtime_error("Main camera has no transform component.");

    SceneData sceneData{
        .projectionMatrix = mainCamera->getProjectionMatrix(renderer.getAspectRatio()),
        .viewMatrix = mainCamera->getViewMatrix(),
        .cameraPosition = cameraTransform->getWorldPosition(),
        .shadowBias = renderSettings.getShadowBias(),
        .directionalLight = {
            .direction = glm::vec3(0.0f, -1.0f, 0.0f),
            .color = glm::vec3(1.0f),
            .intensity = 0.0f
        }
    };

    return RenderContext{
        .scene = scene,
        .renderer = renderer,
        .mainCamera = mainCamera,
        .cameraTransform = cameraTransform,
        .shadowDistance = renderSettings.getShadowDistance(),
        .meshRenderers = scene.getComponents<MeshRenderer>(),
        .sceneData = sceneData
    };
}

void RenderSystem::setupDirectionalLight(RenderContext &ctx)
{
    auto directionalLights = ctx.scene.getComponents<DirectionalLight>();
    if (directionalLights.empty())
        return;

    auto light = directionalLights[0];
    auto lightTransform = light->getGameObject()->getComponent<Transform>();

    ctx.directionalLight = light;
    ctx.sceneData.directionalLight.direction = lightTransform->getForward();
    ctx.sceneData.directionalLight.color = light->getColor();
    ctx.sceneData.directionalLight.intensity = light->getIntensity();
}

void RenderSystem::renderShadowPass(RenderContext &ctx)
{
    if (!ctx.directionalLight)
        return;

    auto lightDirection = ctx.sceneData.directionalLight.direction;
    auto &shadowMap = ctx.directionalLight->getShadowMap();

    auto cameraPosition = ctx.cameraTransform->getWorldPosition();
    auto cameraForward = ctx.cameraTransform->getForward();

    auto center = cameraPosition + cameraForward * ctx.shadowDistance * 0.5f;
    auto lightPos = center - lightDirection * ctx.shadowDistance;

    glm::vec3 up = glm::abs(glm::dot(glm::normalize(lightDirection), glm::vec3(0.0f, 1.0f, 0.0f))) > 0.99f
        ? glm::vec3(0.0f, 0.0f, 1.0f)
        : glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 lightViewMatrix = glm::lookAt(
        lightPos,
        center,
        up
    );

    const float shadowMapResolution = static_cast<float>(shadowMap.getWidth());
    const float texelSize = (ctx.shadowDistance * 2.0f) / shadowMapResolution;

    glm::vec3 centerInLightSpace = glm::vec3(lightViewMatrix * glm::vec4(center, 1.0f));
    centerInLightSpace.x = std::floor(centerInLightSpace.x / texelSize) * texelSize;
    centerInLightSpace.y = std::floor(centerInLightSpace.y / texelSize) * texelSize;

    glm::vec3 snappedCenter = glm::vec3(glm::inverse(lightViewMatrix) * glm::vec4(centerInLightSpace, 1.0f));
    glm::vec3 snappedLightPos = snappedCenter - lightDirection * ctx.shadowDistance;

    lightViewMatrix = glm::lookAt(
        snappedLightPos,
        snappedCenter,
        up
    );

    glm::mat4 lightProjectionMatrix = glm::ortho(
        -ctx.shadowDistance,
        ctx.shadowDistance,
        -ctx.shadowDistance,
        ctx.shadowDistance,
        0.1f,
        ctx.shadowDistance * 4.0f
    );

    shadowMap.setLightSpaceMatrix(lightProjectionMatrix * lightViewMatrix);

    ctx.renderer.beginShadowPass(&shadowMap);

    for (MeshRenderer *meshRenderer : ctx.meshRenderers)
    {
        auto *transform = meshRenderer->getGameObject()->getComponent<Transform>();
        ctx.renderer.drawShadow(*transform, *meshRenderer->getMesh());
    }

    ctx.renderer.endShadowPass();
}

void RenderSystem::collectPointLights(RenderContext &ctx)
{
    auto pointLights = ctx.scene.getComponents<PointLight>();
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
        ctx.sceneData.pointLights.push_back(pointLightData);
    }
}

void RenderSystem::collectSpotLights(RenderContext &ctx)
{
    auto spotLights = ctx.scene.getComponents<Spotlight>();
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
        ctx.sceneData.spotLights.push_back(spotLightData);
    }
}

void RenderSystem::renderMainPass(RenderContext &ctx)
{
    ctx.renderer.beginScene(ctx.sceneData);

    const auto& renderSettings = ctx.scene.getRenderSettings();
    if (renderSettings.getSkyboxEnabled())
    {
        const auto& skybox = renderSettings.getSkybox();
        if (skybox.getCubemap())
            ctx.renderer.drawSkybox(skybox);
    }

    for (MeshRenderer *meshRenderer : ctx.meshRenderers)
    {
        auto *transform = meshRenderer->getGameObject()->getComponent<Transform>();
        ctx.renderer.draw(*transform, *meshRenderer->getMesh(), *meshRenderer->getMaterial());
    }

    ctx.renderer.endScene();
}
