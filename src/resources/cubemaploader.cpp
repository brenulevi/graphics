#include "cubemaploader.h"

std::shared_ptr<Cubemap> CubemapLoader::loadFromFiles(const std::vector<std::string> &faces)
{
    return std::make_shared<Cubemap>(faces);
}
