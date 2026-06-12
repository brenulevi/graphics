#pragma once

#include <memory>
#include <string>
#include <vector>

#include "rendering/cubemap.h"

class CubemapLoader
{
public:
    static std::shared_ptr<Cubemap> loadFromFiles(const std::vector<std::string> &faces);
};
