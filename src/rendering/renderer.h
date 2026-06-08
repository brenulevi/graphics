#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "components/camera.h"
#include "components/transform.h"
#include "rendering/mesh.h"
#include "rendering/material.h"
#include "scenering/scenedata.h"

class Renderer
{
public:
    Renderer();
    ~Renderer() = default;

    void clear() const;

    void beginScene(const SceneData& sceneData);
    void draw(const Transform& transform, const Mesh& mesh, const Material& material);
    void endScene();

    void setViewportSize(float width, float height);

    inline float getAspectRatio() const { return m_viewportSize.x / m_viewportSize.y; }

private:
    glm::vec2 m_viewportSize;
    SceneData m_sceneData;
};