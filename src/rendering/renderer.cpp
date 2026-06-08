#include "renderer.h"

Renderer::Renderer()
    : m_viewportSize(800.0f, 600.0f)
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

void Renderer::beginScene(const SceneData &sceneData)
{
    m_sceneData = sceneData;
}

void Renderer::draw(const Transform &transform, const Mesh &mesh, const Material &material)
{
    auto shader = material.getShader();
    auto diffuse = material.getDiffuse();
    auto specular = material.getSpecular();

    if(!shader)
    {
        throw std::runtime_error("Material has no shader!");
    }

    shader->use();

    if(diffuse)
    {
        diffuse->bind(0);
        shader->setInt("u_Material.diffuse", 0);
    }

    if(specular)
    {
        specular->bind(1);
        shader->setInt("u_Material.specular", 1);
    }

    shader->setFloat("u_Material.shininess", material.getShininess());

    shader->setVec3("u_ViewPos", m_sceneData.cameraPosition);
    shader->setVec3("u_Light.direction", m_sceneData.lightDirection);
    shader->setVec3("u_Light.color", m_sceneData.lightColor);
    shader->setFloat("u_Light.intensity", m_sceneData.lightIntensity);

    shader->setMat4("u_ViewProjection", m_sceneData.viewProjectionMatrix);
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
