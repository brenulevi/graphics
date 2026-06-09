#include "textureloader.h"

std::shared_ptr<Texture> TextureLoader::loadFromFile(const std::string &filepath)
{
    return std::make_shared<Texture>(filepath.c_str());
}