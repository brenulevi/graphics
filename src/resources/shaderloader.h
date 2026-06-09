#pragma once

#include <memory>

#include "rendering/shader.h"

class ShaderLoader
{
public:
    static std::shared_ptr<Shader> loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
};