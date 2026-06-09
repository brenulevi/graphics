#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "rendering/mesh.h"
#include "rendering/texture.h"
#include "rendering/shader.h"
#include "rendering/model.h"

#include "meshloader.h"
#include "textureloader.h"
#include "shaderloader.h"
#include "modelloader.h"

class AssetManager
{
public:
    static void clear();
    
    static std::shared_ptr<Mesh> loadMesh(const std::string& name, const std::string& filepath);
    static std::shared_ptr<Mesh> createMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    static std::shared_ptr<Texture> loadTexture(const std::string& name, const std::string& filepath);
    static std::shared_ptr<Shader> loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    static std::shared_ptr<Model> loadModel(const std::string& name, const std::string& filepath);

    static inline std::shared_ptr<Mesh> getMesh(const std::string& name) { return s_meshes.at(name); }
    static inline std::shared_ptr<Texture> getTexture(const std::string& name) { return s_textures.at(name); }
    static inline std::shared_ptr<Shader> getShader(const std::string& name) { return s_shaders.at(name); }
    static inline std::shared_ptr<Model> getModel(const std::string& name) { return s_models.at(name); }
private:
    static std::unordered_map<std::string, std::shared_ptr<Mesh>> s_meshes;
    static std::unordered_map<std::string, std::shared_ptr<Texture>> s_textures;
    static std::unordered_map<std::string, std::shared_ptr<Shader>> s_shaders;
    static std::unordered_map<std::string, std::shared_ptr<Model>> s_models;
};