#pragma once

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    void pollEvents();
    bool shouldClose();
    void swapBuffers();

private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    std::string m_title;
};