#include "shadowmap.h"

ShadowMap::ShadowMap(int width, int height)
    : m_width(width), m_height(height)
{
    m_depthTexture = std::make_shared<Texture>(m_width, m_height, TextureFormat::Depth);
    m_shadowShader = std::make_shared<Shader>("assets/shaders/shadow.vert", "assets/shaders/shadow.frag");

    m_framebuffer = std::make_shared<Framebuffer>();
    m_framebuffer->setDepthAttachment(m_depthTexture);

    if(!m_framebuffer->isComplete())
    {
        throw std::runtime_error("Failed to create shadow map framebuffer");
    }
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::beginRender() const
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_savedFramebuffer);
    m_framebuffer->bindAndSetViewport();
}

void ShadowMap::endRender() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_savedFramebuffer);
}