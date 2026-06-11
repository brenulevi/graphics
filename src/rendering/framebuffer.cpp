#include "framebuffer.h"

Framebuffer::Framebuffer()
{
    glGenFramebuffers(1, &m_id);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_id);
}

void Framebuffer::setColorAttachment(const std::shared_ptr<Texture> &texture)
{
    m_colorAttachment = texture;

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getID(), 0);
}

void Framebuffer::setDepthAttachment(const std::shared_ptr<Texture> &texture)
{
    m_depthAttachment = texture;

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getID(), 0);

    if(!m_colorAttachment)
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
}

void Framebuffer::setDepthStencilAttachment(const std::shared_ptr<Texture> &texture)
{
    m_depthStencilAttachment = texture;

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->getID(), 0);

    if(!m_colorAttachment)
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
}

bool Framebuffer::isComplete() const
{
    bind();
    bool complete = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    return complete;
}

void Framebuffer::bindAndSetViewport() const
{
    bind();
    auto texture = m_depthAttachment ? m_depthAttachment : m_colorAttachment;

    if(!texture)
    {
        throw std::runtime_error("Framebuffer has no attachments");
    }

    glViewport(0, 0, texture->getWidth(), texture->getHeight());
}
