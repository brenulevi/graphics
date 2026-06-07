#pragma once

#include <glad/gl.h>

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* data, unsigned int size);
    ~IndexBuffer();

    void bind() const;

    inline unsigned int getCount() const { return m_count; }

private:
    unsigned int m_id;
    unsigned int m_count;
};