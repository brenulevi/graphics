#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

#include "mesh.h"
#include "material.h"

struct ModelNode
{
    std::string name;
    
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    std::vector<unsigned int> meshIndices;
    std::vector<std::unique_ptr<ModelNode>> children;
};

struct ModelMesh
{
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};

struct Model
{
public:
    ModelNode rootNode;

    std::vector<ModelMesh> meshes;
};