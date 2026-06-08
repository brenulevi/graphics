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

    shader->setVec3("u_DirectionalLight.direction", m_sceneData.directionalLight.direction);
    shader->setVec3("u_DirectionalLight.color", m_sceneData.directionalLight.color);
    shader->setFloat("u_DirectionalLight.intensity", m_sceneData.directionalLight.intensity);

    shader->setInt("u_NumPointLights", std::min(static_cast<int>(m_sceneData.pointLights.size()), MAX_POINT_LIGHTS));
    for(size_t i = 0; i < m_sceneData.pointLights.size() && i < MAX_POINT_LIGHTS; ++i)
    {
        const auto& pointLight = m_sceneData.pointLights[i];
        shader->setVec3("u_PointLights[" + std::to_string(i) + "].position", pointLight.position);
        shader->setVec3("u_PointLights[" + std::to_string(i) + "].color", pointLight.color);
        shader->setFloat("u_PointLights[" + std::to_string(i) + "].intensity", pointLight.intensity);
        shader->setFloat("u_PointLights[" + std::to_string(i) + "].constant", pointLight.constant);
        shader->setFloat("u_PointLights[" + std::to_string(i) + "].linear", pointLight.linear);
        shader->setFloat("u_PointLights[" + std::to_string(i) + "].quadratic", pointLight.quadratic);
    }

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
