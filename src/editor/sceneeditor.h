#pragma once

#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_internal.h"

#include "components/camera.h"
#include "components/directionallight.h"
#include "components/meshrenderer.h"
#include "components/playercontroller.h"
#include "components/pointlight.h"
#include "components/spotlight.h"
#include "components/transform.h"
#include "rendering/material.h"
#include "scenering/gameobject.h"
#include "scenering/rendersettings.h"
#include "scenering/scene.h"

class SceneEditor
{
public:
    void drawDockspace();
    void drawPanels(Scene& scene);

private:
    void drawHierarchy(Scene& scene);
    void drawInspector(Scene& scene);
    void drawRenderSettings(Scene& scene);
    void drawGameObjectNode(GameObject* gameObject);

    void drawTransform(GameObject* gameObject);
    void drawCamera(GameObject* gameObject);
    void drawMeshRenderer(GameObject* gameObject);
    void drawDirectionalLight(GameObject* gameObject);
    void drawPointLight(GameObject* gameObject);
    void drawSpotLight(GameObject* gameObject);
    void drawPlayerController(GameObject* gameObject);

    bool isInScene(GameObject* gameObject, Scene& scene) const;
    void setupInitialLayout(ImGuiID dockspaceId);

private:
    GameObject* m_selectedGameObject = nullptr;
    bool m_initialLayoutBuilt = false;
};
