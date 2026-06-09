#pragma once

#include <glad/gl.h>
#include <stdexcept>
#include <vector>

struct BufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT: return sizeof(float);
        case GL_UNSIGNED_INT: return sizeof(unsigned int);
        case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
        }
        throw std::runtime_error("Unknown type!");
        return 0;
    }
};

class BufferLayout
{
public:
    BufferLayout() : m_stride(0) {}
    BufferLayout(std::initializer_list<BufferElement> elements)
        : m_elements(elements), m_stride(0)
    {
        for (const auto& element : m_elements)
        {
            m_stride += element.count * BufferElement::getSizeOfType(element.type);
        }
    }
    ~BufferLayout() = default;

    template<typename T>
    void push(unsigned int count);

    inline std::vector<BufferElement> getElements() const { return m_elements; }
    inline unsigned int getStride() const { return m_stride; }

private:
    std::vector<BufferElement> m_elements;
    unsigned int m_stride;
};

template <typename T>
inline void BufferLayout::push(unsigned int count)
{
    throw std::runtime_error("Unsupported type!");
}

template <>
inline void BufferLayout::push<float>(unsigned int count)
{
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_stride += count * BufferElement::getSizeOfType(GL_FLOAT);
}

template <>
inline void BufferLayout::push<unsigned int>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_stride += count * BufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template <>
inline void BufferLayout::push<unsigned char>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_stride += count * BufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}
