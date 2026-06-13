#include "sceneeditor.h"

namespace
{
    bool drawVec3(const char* label, glm::vec3& value, float speed = 0.1f)
    {
        return ImGui::DragFloat3(label, &value.x, speed);
    }
}

void SceneEditor::drawDockspace(Scene& scene, PlayMode& playMode)
{
    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("EditorDockSpaceHost", nullptr, windowFlags);
    ImGui::PopStyleVar(2);

    if (ImGui::BeginMenuBar())
    {
        if (playMode.isPlaying())
        {
            if (ImGui::MenuItem("Stop", "Esc"))
                playMode.stop();
        }
        else if (ImGui::MenuItem("Play", "F5"))
        {
            playMode.start();
        }

        ImGui::EndMenuBar();
    }

    ImGuiID dockspaceId = ImGui::GetID("EditorDockSpace");
    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f));

    if (!m_initialLayoutBuilt)
    {
        setupInitialLayout(dockspaceId);
        m_initialLayoutBuilt = true;
    }

    ImGui::End();
}

void SceneEditor::drawPanels(Scene& scene)
{
    if (m_selectedGameObject && !isInScene(m_selectedGameObject, scene))
        m_selectedGameObject = nullptr;

    drawHierarchy(scene);
    drawInspector(scene);
    drawRenderSettings(scene);
}

void SceneEditor::setupInitialLayout(ImGuiID dockspaceId)
{
    ImGui::DockBuilderRemoveNode(dockspaceId);
    ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->WorkSize);

    ImGuiID dockMain = dockspaceId;
    ImGuiID dockLeft = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Left, 0.2f, nullptr, &dockMain);
    ImGuiID dockRight = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Right, 0.25f, nullptr, &dockMain);
    ImGuiID dockBottom = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Down, 0.2f, nullptr, &dockMain);

    ImGui::DockBuilderDockWindow("Game", dockMain);
    ImGui::DockBuilderDockWindow("Hierarchy", dockLeft);
    ImGui::DockBuilderDockWindow("Inspector", dockRight);
    ImGui::DockBuilderDockWindow("Render Settings", dockBottom);
    ImGui::DockBuilderFinish(dockspaceId);
}

void SceneEditor::drawHierarchy(Scene& scene)
{
    ImGui::Begin("Hierarchy");

    for (GameObject* gameObject : scene.getRootGameObjects())
        drawGameObjectNode(gameObject);

    ImGui::End();
}

void SceneEditor::drawGameObjectNode(GameObject* gameObject)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (gameObject->getChildren().empty())
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    if (m_selectedGameObject == gameObject)
        flags |= ImGuiTreeNodeFlags_Selected;

    const bool opened = ImGui::TreeNodeEx(gameObject->getName().c_str(), flags);

    if (ImGui::IsItemClicked())
        m_selectedGameObject = gameObject;

    if (opened && !gameObject->getChildren().empty())
    {
        for (GameObject* child : gameObject->getChildren())
            drawGameObjectNode(child);

        ImGui::TreePop();
    }
}

void SceneEditor::drawInspector(Scene& scene)
{
    ImGui::Begin("Inspector");

    if (!m_selectedGameObject)
    {
        ImGui::TextUnformatted("Select a GameObject in the Hierarchy.");
        ImGui::End();
        return;
    }

    ImGui::Text("Name: %s", m_selectedGameObject->getName().c_str());

    if (auto* camera = m_selectedGameObject->getComponent<Camera>();
        camera != nullptr && camera == scene.getMainCamera())
    {
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Main Camera");
    }

    drawTransform(m_selectedGameObject);
    drawCamera(m_selectedGameObject);
    drawMeshRenderer(m_selectedGameObject);
    drawDirectionalLight(m_selectedGameObject);
    drawPointLight(m_selectedGameObject);
    drawSpotLight(m_selectedGameObject);
    drawPlayerController(m_selectedGameObject);

    ImGui::End();
}

void SceneEditor::drawRenderSettings(Scene& scene)
{
    ImGui::Begin("Render Settings");

    RenderSettings& settings = scene.getRenderSettings();

    bool skyboxEnabled = settings.getSkyboxEnabled();
    if (ImGui::Checkbox("Skybox", &skyboxEnabled))
        settings.setSkyboxEnabled(skyboxEnabled);

    float shadowDistance = settings.getShadowDistance();
    if (ImGui::DragFloat("Shadow Distance", &shadowDistance, 1.0f, 1.0f, 500.0f))
        settings.setShadowDistance(shadowDistance);

    float shadowBias = settings.getShadowBias();
    if (ImGui::DragFloat("Shadow Bias", &shadowBias, 0.0001f, 0.0f, 0.05f, "%.4f"))
        settings.setShadowBias(shadowBias);

    ImGui::End();
}

void SceneEditor::drawTransform(GameObject* gameObject)
{
    auto* transform = gameObject->getComponent<Transform>();
    if (!transform)
        return;

    if (!ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    glm::vec3 position = transform->getLocalPosition();
    if (drawVec3("Position", position))
        transform->setLocalPosition(position);

    glm::vec3 rotation = transform->getLocalRotationEuler();
    if (drawVec3("Rotation", rotation, 0.5f))
        transform->setLocalRotationEuler(rotation);

    glm::vec3 scale = transform->getLocalScale();
    if (drawVec3("Scale", scale, 0.01f))
        transform->setLocalScale(scale);

    ImGui::Separator();
    glm::vec3 worldPosition = transform->getWorldPosition();
    ImGui::BeginDisabled();
    ImGui::DragFloat3("World Position", &worldPosition.x, 0.0f);
    glm::vec3 forward = transform->getForward();
    ImGui::DragFloat3("Forward", &forward.x, 0.0f);
    ImGui::EndDisabled();
}

void SceneEditor::drawCamera(GameObject* gameObject)
{
    auto* camera = gameObject->getComponent<Camera>();
    if (!camera)
        return;

    if (!ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    float fov = camera->getFOV();
    if (ImGui::DragFloat("FOV", &fov, 0.5f, 1.0f, 179.0f))
        camera->setFOV(fov);

    float nearPlane = camera->getNearPlane();
    if (ImGui::DragFloat("Near Plane", &nearPlane, 0.01f, 0.01f, 10.0f))
        camera->setNearPlane(nearPlane);

    float farPlane = camera->getFarPlane();
    if (ImGui::DragFloat("Far Plane", &farPlane, 1.0f, 10.0f, 10000.0f))
        camera->setFarPlane(farPlane);
}

void SceneEditor::drawMeshRenderer(GameObject* gameObject)
{
    auto* meshRenderer = gameObject->getComponent<MeshRenderer>();
    if (!meshRenderer)
        return;

    if (!ImGui::CollapsingHeader("Mesh Renderer"))
        return;

    ImGui::Text("Mesh: %s", meshRenderer->getMesh() ? "assigned" : "none");
    ImGui::Text("Material: %s", meshRenderer->getMaterial() ? "assigned" : "none");

    if (auto* material = meshRenderer->getMaterial())
    {
        ImGui::Text("Type: %s", material->getTypeName());

        if (material->getType() == MaterialType::Standard)
        {
            float shininess = material->getShininess();
            if (ImGui::DragFloat("Shininess", &shininess, 0.5f, 1.0f, 256.0f))
                material->setShininess(shininess);
        }
        else if (material->getType() == MaterialType::Unlit)
        {
            glm::vec3 tint = material->getTint();
            if (ImGui::ColorEdit3("Tint", &tint.x))
                material->setTint(tint);
        }
    }
}

void SceneEditor::drawDirectionalLight(GameObject* gameObject)
{
    auto* light = gameObject->getComponent<DirectionalLight>();
    if (!light)
        return;

    if (!ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    glm::vec3 color = light->getColor();
    if (ImGui::ColorEdit3("Color", &color.x))
        light->setColor(color);

    float intensity = light->getIntensity();
    if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, 10.0f))
        light->setIntensity(intensity);
}

void SceneEditor::drawPointLight(GameObject* gameObject)
{
    auto* light = gameObject->getComponent<PointLight>();
    if (!light)
        return;

    if (!ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    glm::vec3 color = light->getColor();
    if (ImGui::ColorEdit3("Color", &color.x))
        light->setColor(color);

    float intensity = light->getIntensity();
    if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, 10.0f))
        light->setIntensity(intensity);

    float constant = light->getConstant();
    if (ImGui::DragFloat("Constant", &constant, 0.001f, 0.0f, 2.0f))
        light->setConstant(constant);

    float linear = light->getLinear();
    if (ImGui::DragFloat("Linear", &linear, 0.001f, 0.0f, 2.0f))
        light->setLinear(linear);

    float quadratic = light->getQuadratic();
    if (ImGui::DragFloat("Quadratic", &quadratic, 0.0001f, 0.0f, 2.0f))
        light->setQuadratic(quadratic);
}

void SceneEditor::drawSpotLight(GameObject* gameObject)
{
    auto* light = gameObject->getComponent<Spotlight>();
    if (!light)
        return;

    if (!ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    glm::vec3 color = light->getColor();
    if (ImGui::ColorEdit3("Color", &color.x))
        light->setColor(color);

    float intensity = light->getIntensity();
    if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.0f, 10.0f))
        light->setIntensity(intensity);
}

void SceneEditor::drawPlayerController(GameObject* gameObject)
{
    auto* controller = gameObject->getComponent<PlayerController>();
    if (!controller)
        return;

    if (!ImGui::CollapsingHeader("Player Controller"))
        return;

    ImGui::TextUnformatted("Runs during Play mode.");

    float moveSpeed = controller->getMoveSpeed();
    if (ImGui::DragFloat("Move Speed", &moveSpeed, 0.1f, 0.1f, 50.0f))
        controller->setMoveSpeed(moveSpeed);

    float sensitivity = controller->getSensitivity();
    if (ImGui::DragFloat("Sensitivity", &sensitivity, 0.001f, 0.001f, 1.0f))
        controller->setSensitivity(sensitivity);
}

bool SceneEditor::isInScene(GameObject* gameObject, Scene& scene) const
{
    for (GameObject* object : scene.getGameObjects())
    {
        if (object == gameObject)
            return true;
    }
    return false;
}
