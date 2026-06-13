#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "shader.h"
#include "texture.h"

enum class MaterialType
{
    Standard,
    Unlit
};

class Material
{
public:
    Material() = default;

    Material(
        std::shared_ptr<Shader> shader,
        std::shared_ptr<Texture> diffuse,
        std::shared_ptr<Texture> specular,
        float shininess
    );

    static std::shared_ptr<Material> createStandard(
        std::shared_ptr<Shader> shader,
        std::shared_ptr<Texture> diffuse,
        std::shared_ptr<Texture> specular,
        float shininess
    );

    static std::shared_ptr<Material> createUnlit(
        std::shared_ptr<Shader> shader,
        std::shared_ptr<Texture> colorMap,
        const glm::vec3& tint = glm::vec3(1.0f)
    );

    inline MaterialType getType() const { return m_type; }
    const char* getTypeName() const;

    inline std::shared_ptr<Shader> getShader() const { return m_shader; }
    inline void setShader(std::shared_ptr<Shader> shader) { m_shader = std::move(shader); }

    inline std::shared_ptr<Texture> getDiffuse() const { return m_diffuse; }
    inline void setDiffuse(std::shared_ptr<Texture> diffuse) { m_diffuse = std::move(diffuse); }

    inline std::shared_ptr<Texture> getSpecular() const { return m_specular; }
    inline void setSpecular(std::shared_ptr<Texture> specular) { m_specular = std::move(specular); }

    inline float getShininess() const { return m_shininess; }
    inline void setShininess(float shininess) { m_shininess = shininess; }

    inline const glm::vec3& getTint() const { return m_tint; }
    inline void setTint(const glm::vec3& tint) { m_tint = tint; }

private:
    MaterialType m_type = MaterialType::Standard;
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Texture> m_diffuse;
    std::shared_ptr<Texture> m_specular;
    float m_shininess = 32.0f;
    glm::vec3 m_tint = glm::vec3(1.0f);
};
