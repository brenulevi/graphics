#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    m_vertexArray = std::make_unique<VertexArray>();
    m_vertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
    m_indexBuffer = std::make_unique<IndexBuffer>(indices.data(), indices.size() * sizeof(unsigned int));
    m_vertexArray->addBuffer(*m_vertexBuffer, Vertex::getBufferLayout());
}

Mesh::~Mesh()
{
}

void Mesh::bind() const
{
    m_vertexArray->bind();
    m_indexBuffer->bind();
}
