#include "textureloader.h"

std::shared_ptr<Texture> TextureLoader::loadFromFile(const std::string &filepath)
{
    return std::make_shared<Texture>(filepath.c_str());
}

std::shared_ptr<Texture> TextureLoader::createTexture(int width, int height, TextureFormat format)
{
    return std::make_shared<Texture>(width, height, format);
}
