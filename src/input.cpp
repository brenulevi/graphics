#include "input.h"

bool Input::m_currentKeys[GLFW_KEY_LAST] = { false };
bool Input::m_previousKeys[GLFW_KEY_LAST] = { false };
bool Input::m_currentMouseButtons[GLFW_MOUSE_BUTTON_LAST] = { false };
bool Input::m_previousMouseButtons[GLFW_MOUSE_BUTTON_LAST] = { false };
bool Input::m_uiCapturing = false;
bool Input::m_gameViewFocused = false;
glm::vec2 Input::m_currentMousePosition = glm::vec2(0.0f);
glm::vec2 Input::m_previousMousePosition = glm::vec2(0.0f);

void Input::update()
{
    memcpy(m_previousKeys, m_currentKeys, sizeof(m_currentKeys));
    memcpy(m_previousMouseButtons, m_currentMouseButtons, sizeof(m_currentMouseButtons));
    
    m_previousMousePosition = m_currentMousePosition;
    double x, y;
    glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
    m_currentMousePosition = glm::vec2(
        static_cast<float>(x),
        static_cast<float>(y)
    );

    for (int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        m_currentKeys[i] = glfwGetKey(glfwGetCurrentContext(), i) == GLFW_PRESS;
    }

    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i)
    {
        m_currentMouseButtons[i] = glfwGetMouseButton(glfwGetCurrentContext(), i) == GLFW_PRESS;
    }
}