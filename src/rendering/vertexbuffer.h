#pragma once

#include <glad/gl.h>

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;

private:
    unsigned int m_id;
};