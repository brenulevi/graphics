#pragma once

#include <stdexcept>
#include <cmath>
#include <vector>

#include "scenering/scene.h"
#include "scenering/scenedata.h"
#include "scenering/skybox.h"
#include "rendering/renderer.h"
#include "components/camera.h"
#include "components/meshrenderer.h"
#include "components/transform.h"
#include "components/directionallight.h"
#include "components/pointlight.h"
#include "components/spotlight.h"

class RenderSystem
{
public:
    RenderSystem() = default;
    ~RenderSystem() = default;

    void render(
        Scene &scene,
        Renderer &renderer);

private:
    struct RenderContext
    {
        Scene &scene;
        Renderer &renderer;
        Camera *mainCamera;
        Transform *cameraTransform;
        float shadowDistance;
        DirectionalLight *directionalLight = nullptr;
        std::vector<MeshRenderer *> meshRenderers;
        SceneData sceneData;
    };

    RenderContext buildRenderContext(Scene &scene, Renderer &renderer);
    void setupDirectionalLight(RenderContext &ctx);
    void renderShadowPass(RenderContext &ctx);
    void collectPointLights(RenderContext &ctx);
    void collectSpotLights(RenderContext &ctx);
    void renderMainPass(RenderContext &ctx);
};
