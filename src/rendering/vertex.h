#pragma once

#include <glm/glm.hpp>

#include "bufferlayout.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    static BufferLayout getBufferLayout()
    {
        return BufferLayout({
            { GL_FLOAT, 3, GL_FALSE },
            { GL_FLOAT, 3, GL_FALSE },
            { GL_FLOAT, 2, GL_FALSE }
        });
    }
};