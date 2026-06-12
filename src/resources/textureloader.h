#pragma once

#include <memory>

#include "rendering/texture.h"

class TextureLoader
{
public:
    static std::shared_ptr<Texture> loadFromFile(const std::string& filepath);
    static std::shared_ptr<Texture> createTexture(int width, int height, TextureFormat format);
};
