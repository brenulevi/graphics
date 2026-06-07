#pragma once

#include <memory>

#include "shader.h"
#include "texture.h"

class Material
{
public:
    Material() = default;
    Material(
        std::shared_ptr<Shader> shader,
        std::shared_ptr<Texture> diffuse,
        std::shared_ptr<Texture> specular,
        float shininess
    )
        : m_shader(shader), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {}
    ~Material() = default;

    inline std::shared_ptr<Shader> getShader() const { return m_shader; }
    inline void setShader(std::shared_ptr<Shader> shader) { m_shader = std::move(shader); }

    inline std::shared_ptr<Texture> getDiffuse() const { return m_diffuse; }
    inline void setDiffuse(std::shared_ptr<Texture> diffuse) { m_diffuse = std::move(diffuse); }

    inline std::shared_ptr<Texture> getSpecular() const { return m_specular; }
    inline void setSpecular(std::shared_ptr<Texture> specular) { m_specular = std::move(specular); }

    inline float getShininess() const { return m_shininess; }
    inline void setShininess(float shininess) { m_shininess = shininess; }

private:
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Texture> m_diffuse;
    std::shared_ptr<Texture> m_specular;
    float m_shininess;
};
