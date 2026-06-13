#include "window.h"

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    glfwSwapInterval(0);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        win->m_width = width;
        win->m_height = height;

        if (win->m_resizeCallback)
        {
            win->m_resizeCallback(width, height);
        }
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (win->m_closeCallback)
        {
            win->m_closeCallback();
        }
    });
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

void Window::setTitle(const std::string &title)
{
    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

void Window::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
    glfwSetWindowSize(m_window, m_width, m_height);
}
