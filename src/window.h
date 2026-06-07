#pragma once

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <functional>

class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    inline void setCloseCallback(const std::function<void()>& callback) { m_closeCallback = callback; }
    inline void setResizeCallback(const std::function<void(int, int)>& callback) { m_resizeCallback = callback; }

    void pollEvents();
    void swapBuffers();

    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }
    inline const std::string& getTitle() const { return m_title; }

    void setTitle(const std::string& title);
    void setSize(int width, int height);

private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    std::string m_title;

    std::function<void()> m_closeCallback;
    std::function<void(int, int)> m_resizeCallback;
};