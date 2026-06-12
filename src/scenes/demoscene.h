#pragma once

#include "scenering/iscenesetup.h"

class DemoScene : public ISceneSetup
{
public:
    RenderSettings createRenderSettings() override;
    void build(Scene& scene) override;
};
