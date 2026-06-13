#pragma once

#include "components/camera.h"
#include "components/directionallight.h"
#include "components/meshrenderer.h"
#include "components/playercontroller.h"
#include "components/pointlight.h"
#include "components/transform.h"
#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/vertex.h"
#include "resources/assetmanager.h"
#include "scenering/iscenesetup.h"
#include "scenering/rendersettings.h"
#include "scenering/scene.h"
#include "scenering/skybox.h"

class DemoScene : public ISceneSetup
{
public:
    RenderSettings createRenderSettings() override;
    void build(Scene& scene) override;
};
