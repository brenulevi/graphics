#pragma once

#include <memory>

#include "rendering/mesh.h"

class MeshLoader
{
public:
    static std::shared_ptr<Mesh> createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};