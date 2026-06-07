#pragma once

#include <vector>
#include <memory>

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertex.h"

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void bind() const;

    inline VertexArray& getVertexArray() const { return *m_vertexArray; }
    inline VertexBuffer& getVertexBuffer() const { return *m_vertexBuffer; }
    inline IndexBuffer& getIndexBuffer() const { return *m_indexBuffer; }

private:
    std::unique_ptr<VertexArray> m_vertexArray;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
};