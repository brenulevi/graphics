#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "components/camera.h"
#include "components/transform.h"
#include "rendering/mesh.h"
#include "rendering/material.h"

class Renderer
{
public:
    Renderer();
    ~Renderer() = default;

    void clear() const;

    void beginScene(const Transform& cameraTransform, const Camera& camera);
    void draw(const Transform& transform, const Mesh& mesh, const Material& material);
    void endScene();

    void setViewportSize(float width, float height);

private:
    glm::mat4 m_viewProjectionMatrix;
    glm::vec2 m_viewportSize;
};