#include "meshloader.h"

std::shared_ptr<Mesh> MeshLoader::createMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    return std::make_shared<Mesh>(vertices, indices);
}