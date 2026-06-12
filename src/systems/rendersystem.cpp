#include "rendersystem.h"

void RenderSystem::render(
    Scene &scene,
    float shadowDistance,
    Renderer &renderer)
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
        .projectionMatrix = mainCamera->getProjectionMatrix(renderer.getAspectRatio()),
        .viewMatrix = mainCamera->getViewMatrix(),
        .cameraPosition = cameraTransform->getWorldPosition(),
        .directionalLight = {
            .direction = glm::vec3(0.0f, -1.0f, 0.0f),
            .color = glm::vec3(1.0f),
            .intensity = 0.0f
        }
    };

    auto meshRenderers = scene.getComponents<MeshRenderer>();

    auto directionalLight = scene.getComponents<DirectionalLight>();
    if (!directionalLight.empty())
    {
        auto light = directionalLight[0];
        auto lightTransform = light->getGameObject()->getComponent<Transform>();
        auto lightDirection = lightTransform->getForward();

        sceneData.directionalLight.direction = lightDirection;
        sceneData.directionalLight.color = light->getColor();
        sceneData.directionalLight.intensity = light->getIntensity();

        auto& shadowMap = light->getShadowMap();
        
        auto cameraPosition = cameraTransform->getWorldPosition();
        auto cameraForward = cameraTransform->getForward();

        auto center = cameraPosition + cameraForward * shadowDistance * 0.5f;
        auto lightPos = center - lightDirection * shadowDistance;

        glm::vec3 up = glm::abs(glm::dot(glm::normalize(lightDirection), glm::vec3(0.0f, 1.0f, 0.0f))) > 0.99f
            ? glm::vec3(0.0f, 0.0f, 1.0f)
            : glm::vec3(0.0f, 1.0f, 0.0f);

        glm::mat4 lightViewMatrix = glm::lookAt(
            lightPos,
            center,
            up
        );

        const float shadowMapResolution = static_cast<float>(shadowMap.getWidth());
        const float texelSize = (shadowDistance * 2.0f) / shadowMapResolution;

        glm::vec3 centerInLightSpace = glm::vec3(lightViewMatrix * glm::vec4(center, 1.0f));
        centerInLightSpace.x = std::floor(centerInLightSpace.x / texelSize) * texelSize;
        centerInLightSpace.y = std::floor(centerInLightSpace.y / texelSize) * texelSize;

        glm::vec3 snappedCenter = glm::vec3(glm::inverse(lightViewMatrix) * glm::vec4(centerInLightSpace, 1.0f));
        glm::vec3 snappedLightPos = snappedCenter - lightDirection * shadowDistance;

        lightViewMatrix = glm::lookAt(
            snappedLightPos,
            snappedCenter,
            up
        );

        glm::mat4 lightProjectionMatrix = glm::ortho(
            -shadowDistance,
            shadowDistance,
            -shadowDistance,
            shadowDistance,
            0.1f,
            shadowDistance * 4.0f
        );

        shadowMap.setLightSpaceMatrix(lightProjectionMatrix * lightViewMatrix);

        renderer.beginShadowPass(&shadowMap);

        for (MeshRenderer *meshRenderer : meshRenderers)
        {
            auto *transform = meshRenderer->getGameObject()->getComponent<Transform>();
            renderer.drawShadow(*transform, *meshRenderer->getMesh());
        }

        renderer.endShadowPass();
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

    auto skybox = scene.getComponents<Skybox>();
    if (!skybox.empty())
    {
        auto skyboxComponent = skybox[0];
        auto skyboxTransform = skyboxComponent->getGameObject()->getComponent<Transform>();
        renderer.drawSkybox(*skyboxTransform, *skyboxComponent->getCubemap());
    }

    for (MeshRenderer *meshRenderer : meshRenderers)
    {
        auto *transform = meshRenderer->getGameObject()->getComponent<Transform>();
        renderer.draw(*transform, *meshRenderer->getMesh(), *meshRenderer->getMaterial());
    }

    renderer.endScene();
}