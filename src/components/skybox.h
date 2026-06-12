#pragma once

#include <memory>

#include "component.h"
#include "rendering/cubemap.h"

class Skybox : public Component
{
public:
    Skybox(GameObject* owner, std::shared_ptr<Cubemap> cubemap)
        : Component(owner), m_cubemap(std::move(cubemap)) {}
    ~Skybox() = default;

    inline Cubemap* getCubemap() const { return m_cubemap.get(); }
    inline void setCubemap(std::shared_ptr<Cubemap> cubemap) { m_cubemap = std::move(cubemap); }

private:
    std::shared_ptr<Cubemap> m_cubemap;
};