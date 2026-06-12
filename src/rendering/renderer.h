#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>

#include "components/camera.h"
#include "components/transform.h"
#include "rendering/mesh.h"
#include "rendering/material.h"
#include "scenering/scenedata.h"
#include "rendering/shadowmap.h"
#include "scenering/skybox.h"

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

class Renderer
{
public:
    Renderer();
    ~Renderer() = default;

    void clear() const;

    void beginScene(const SceneData& sceneData);
    void draw(const Transform& transform, const Mesh& mesh, const Material& material);
    void endScene();
    
    void beginShadowPass(ShadowMap* shadowMap);
    void drawShadow(const Transform& transform, const Mesh& mesh);
    void endShadowPass();

    void drawSkybox(const Skybox& skybox);

    void setViewportSize(float width, float height);

    inline float getAspectRatio() const { return m_viewportSize.x / m_viewportSize.y; }

private:
    void setupSkybox();

private:
    glm::vec2 m_viewportSize;
    SceneData m_sceneData;
    ShadowMap* m_shadowMap;

    std::shared_ptr<Shader> m_skyboxShader;
    std::shared_ptr<Mesh> m_skyboxMesh;
};