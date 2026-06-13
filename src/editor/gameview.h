#pragma once

#include <memory>

#include <glad/gl.h>

#include "imgui.h"
#include "rendering/framebuffer.h"
#include "rendering/texture.h"

struct GameViewState
{
    int width = 0;
    int height = 0;
    bool isOpen = false;
};

class GameView
{
public:
    GameViewState begin();
    void bind() const;
    void unbind() const;
    void end();

    bool isFocused() const { return m_isFocused; }
    bool isHovered() const { return m_isHovered; }
    bool isActive() const { return m_isFocused || m_isHovered; }

private:
    void resize(int width, int height);

private:
    std::unique_ptr<Framebuffer> m_framebuffer;
    std::shared_ptr<Texture> m_colorTexture;
    std::shared_ptr<Texture> m_depthTexture;

    int m_width = 0;
    int m_height = 0;
    int m_displayWidth = 0;
    int m_displayHeight = 0;
    bool m_isOpen = false;
    bool m_isFocused = false;
    bool m_isHovered = false;
};
