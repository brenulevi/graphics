#include "renderer.h"

Renderer::Renderer()
    : m_viewProjectionMatrix(1.0f), m_viewportSize(800.0f, 600.0f)
{
    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD!");
    }

    glEnable(GL_DEPTH_TEST);
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::beginScene(const Transform &cameraTransform, const Camera &camera)
{
    float aspectRatio = m_viewportSize.x / m_viewportSize.y;
    m_viewProjectionMatrix = camera.getProjectionMatrix(aspectRatio) * cameraTransform.getViewMatrix();
}

void Renderer::draw(const Transform &transform, const Mesh &mesh, const Material &material)
{
    auto shader = material.getShader();

    if(!shader)
    {
        throw std::runtime_error("Material has no shader!");
    }

    shader->use();

    shader->setVec3("u_Color", material.getColor());

    shader->setMat4("u_ViewProjection", m_viewProjectionMatrix);
    shader->setMat4("u_Model", transform.getModelMatrix());

    mesh.bind();
    glDrawElements(GL_TRIANGLES, mesh.getIndexBuffer().getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::endScene()
{
}

void Renderer::setViewportSize(float width, float height)
{
    m_viewportSize = { width, height };

    glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
}
