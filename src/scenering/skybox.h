#pragma once

#include <memory>

#include "rendering/cubemap.h"

class Skybox
{
public:
    Skybox(std::shared_ptr<Cubemap> cubemap)
        : m_cubemap(std::move(cubemap)) {}
    ~Skybox() = default;

    inline std::shared_ptr<Cubemap> getCubemap() const { return m_cubemap; }
    inline void setCubemap(std::shared_ptr<Cubemap> cubemap) { m_cubemap = std::move(cubemap); }

private:
    std::shared_ptr<Cubemap> m_cubemap;
};