#include "gameview.h"

GameViewState GameView::begin()
{
    GameViewState state;

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoNavInputs |
        ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    m_isOpen = ImGui::Begin("Game", nullptr, windowFlags);
    ImGui::PopStyleVar();

    state.isOpen = m_isOpen;
    if (!m_isOpen)
        return state;

    ImVec2 size = ImGui::GetContentRegionAvail();
    state.width = static_cast<int>(size.x);
    state.height = static_cast<int>(size.y);
    m_displayWidth = state.width;
    m_displayHeight = state.height;

    if (state.width > 0 && state.height > 0)
        resize(state.width, state.height);

    return state;
}

void GameView::bind() const
{
    if (!m_framebuffer || !m_colorTexture)
        return;

    m_framebuffer->bindAndSetViewport();
    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
}

void GameView::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameView::end()
{
    if (!m_isOpen)
        return;

    if (m_colorTexture && m_displayWidth > 0 && m_displayHeight > 0)
    {
        ImGui::Image(
            (ImTextureID)(intptr_t)m_colorTexture->getID(),
            ImVec2(static_cast<float>(m_displayWidth), static_cast<float>(m_displayHeight)),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f)
        );
    }

    m_isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
    m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

    ImGui::End();
}

void GameView::resize(int width, int height)
{
    if (width == m_width && height == m_height)
        return;

    m_width = width;
    m_height = height;

    if (!m_framebuffer)
        m_framebuffer = std::make_unique<Framebuffer>();

    m_colorTexture = std::make_shared<Texture>(width, height, TextureFormat::RGBA);
    m_depthTexture = std::make_shared<Texture>(width, height, TextureFormat::Depth);

    m_framebuffer->setColorAttachment(m_colorTexture);
    m_framebuffer->setDepthAttachment(m_depthTexture);

    if (!m_framebuffer->isComplete())
        throw std::runtime_error("Game view framebuffer is incomplete.");
}
