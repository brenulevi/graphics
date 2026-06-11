#pragma once

#include <glad/gl.h>
#include <memory>
#include <stdexcept>

#include "texture.h"

class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();

    inline void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_id); }
    inline void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void setColorAttachment(const std::shared_ptr<Texture> &texture);
    void setDepthAttachment(const std::shared_ptr<Texture> &texture);

    inline std::shared_ptr<Texture> getColorAttachment() const { return m_colorAttachment; }
    inline std::shared_ptr<Texture> getDepthAttachment() const { return m_depthAttachment; }

    bool isComplete() const;

    void bindAndSetViewport() const;

private:
    unsigned int m_id;

    std::shared_ptr<Texture> m_colorAttachment;
    std::shared_ptr<Texture> m_depthAttachment;
};
