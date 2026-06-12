#pragma once

#include "rendersettings.h"

class Scene;

class ISceneSetup
{
public:
    virtual ~ISceneSetup() = default;

    virtual RenderSettings createRenderSettings() { return RenderSettings{}; }
    virtual void build(Scene& scene) = 0;
};
