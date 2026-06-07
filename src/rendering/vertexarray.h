#pragma once

#include <glad/gl.h>

#include "vertexbuffer.h"
#include "bufferlayout.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void addBuffer(const VertexBuffer& vb, const BufferLayout& layout);

private:
    unsigned int m_id;
};