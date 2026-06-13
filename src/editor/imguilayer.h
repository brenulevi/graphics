#pragma once

#include <memory>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "window.h"

class ImGuiLayer
{
public:
    explicit ImGuiLayer(Window& window);
    ~ImGuiLayer();

    void beginFrame();
    void endFrame(int viewportWidth, int viewportHeight);

private:
    Window& m_window;
    bool m_initialized = false;
};
