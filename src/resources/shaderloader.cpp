#include "shaderloader.h"

std::shared_ptr<Shader> ShaderLoader::loadFromFile(const std::string &vertexPath, const std::string &fragmentPath)
{
    return std::make_shared<Shader>(vertexPath.c_str(), fragmentPath.c_str());
}