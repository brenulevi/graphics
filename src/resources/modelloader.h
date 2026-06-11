#pragma once

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "rendering/model.h"
#include "assetmanager.h"

class ModelLoader
{
public:
    static std::shared_ptr<Model> loadFromFile(const std::string& filepath);

private:
    static ModelNode processNode(const aiNode* node, const aiScene* scene, Model& model);
    static ModelMesh processMesh(const aiMesh* mesh, Model& model);
    static std::shared_ptr<Material> processMaterial(const aiMaterial* material, std::string& directory);
};