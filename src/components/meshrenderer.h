#pragma once

#include <memory>

#include "rendering/mesh.h"
#include "rendering/material.h"
#include "component.h"

class MeshRenderer : public Component
{
public:
    MeshRenderer(GameObject* owner, std::shared_ptr<Mesh> mesh = nullptr, std::shared_ptr<Material> material = nullptr)
        : Component(owner), m_mesh(mesh), m_material(material) {}
    ~MeshRenderer() = default;

    inline Mesh* getMesh() const { return m_mesh.get(); }
    inline void setMesh(std::shared_ptr<Mesh> mesh) { m_mesh = std::move(mesh); }

    inline Material* getMaterial() const { return m_material.get(); }
    inline void setMaterial(std::shared_ptr<Material> material) { m_material = std::move(material); }

private:
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
};