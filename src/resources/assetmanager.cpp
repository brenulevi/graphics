#include "assetmanager.h"

std::unordered_map<std::string, std::shared_ptr<Mesh>> AssetManager::s_meshes;
std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::s_textures;
std::unordered_map<std::string, std::shared_ptr<Shader>> AssetManager::s_shaders;
std::unordered_map<std::string, std::shared_ptr<Model>> AssetManager::s_models;

void AssetManager::clear()
{
    s_models.clear();
    s_shaders.clear();
    s_textures.clear();
    s_meshes.clear();
}

std::shared_ptr<Mesh> AssetManager::createMesh(const std::string &name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    auto it = s_meshes.find(name);
    if (it != s_meshes.end())
    {
        return it->second;
    }

    auto mesh = MeshLoader::createMesh(vertices, indices);
    s_meshes[name] = mesh;
    return mesh;
}

std::shared_ptr<Texture> AssetManager::loadTexture(const std::string &name, const std::string &filepath)
{
    auto it = s_textures.find(name);
    if (it != s_textures.end())
    {
        return it->second;
    }

    auto texture = TextureLoader::loadFromFile(filepath);
    s_textures[name] = texture;
    return texture;
}

std::shared_ptr<Texture> AssetManager::createTexture(const std::string &name, int width, int height, TextureFormat format)
{
    auto it = s_textures.find(name);
    if (it != s_textures.end())
    {
        return it->second;
    }

    auto texture = std::make_shared<Texture>(width, height, format);
    s_textures[name] = texture;
    return texture;
}

std::shared_ptr<Shader> AssetManager::loadShader(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath)
{
    auto it = s_shaders.find(name);
    if (it != s_shaders.end())
    {
        return it->second;
    }

    auto shader = ShaderLoader::loadFromFile(vertexPath, fragmentPath);
    s_shaders[name] = shader;
    return shader;
}

std::shared_ptr<Model> AssetManager::loadModel(const std::string &name, const std::string &filepath)
{
    auto it = s_models.find(name);
    if (it != s_models.end())
    {
        return it->second;
    }

    auto model = ModelLoader::loadFromFile(filepath);
    s_models[name] = model;
    return model;
}