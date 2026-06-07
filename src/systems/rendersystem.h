#pragma once

#include <stdexcept>

#include "scenering/scene.h"
#include "rendering/renderer.h"
#include "components/camera.h"
#include "components/meshrenderer.h"

class RenderSystem
{
public:
    RenderSystem() = default;
    ~RenderSystem() = default;

    void render(Scene &scene, Renderer &renderer);
};