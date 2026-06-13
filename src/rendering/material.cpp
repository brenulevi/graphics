#include "material.h"

Material::Material(
    std::shared_ptr<Shader> shader,
    std::shared_ptr<Texture> diffuse,
    std::shared_ptr<Texture> specular,
    float shininess
)
    : m_type(MaterialType::Standard),
      m_shader(std::move(shader)),
      m_diffuse(std::move(diffuse)),
      m_specular(std::move(specular)),
      m_shininess(shininess)
{
}

std::shared_ptr<Material> Material::createStandard(
    std::shared_ptr<Shader> shader,
    std::shared_ptr<Texture> diffuse,
    std::shared_ptr<Texture> specular,
    float shininess
)
{
    return std::make_shared<Material>(
        std::move(shader),
        std::move(diffuse),
        std::move(specular),
        shininess
    );
}

std::shared_ptr<Material> Material::createUnlit(
    std::shared_ptr<Shader> shader,
    std::shared_ptr<Texture> colorMap,
    const glm::vec3& tint
)
{
    auto material = std::make_shared<Material>();
    material->m_type = MaterialType::Unlit;
    material->m_shader = std::move(shader);
    material->m_diffuse = std::move(colorMap);
    material->m_tint = tint;
    return material;
}

const char* Material::getTypeName() const
{
    switch (m_type)
    {
    case MaterialType::Standard:
        return "Standard";
    case MaterialType::Unlit:
        return "Unlit";
    default:
        return "Unknown";
    }
}
