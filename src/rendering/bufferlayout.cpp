#include "bufferlayout.h"

BufferLayout::BufferLayout()
    : m_stride(0)
{
}

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : m_elements(elements), m_stride(0)
{
    for (const auto& element : m_elements)
    {
        m_stride += element.count * BufferElement::getSizeOfType(element.type);
    }
}

BufferLayout::~BufferLayout()
{
}