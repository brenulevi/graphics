#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstring>

class Input
{
public:
    static void update();

    static inline glm::vec2 getMousePosition() { return m_currentMousePosition; }
    static inline glm::vec2 getMouseDelta() { return m_currentMousePosition - m_previousMousePosition; }

    static inline bool isKeyPressed(int key) { return m_currentKeys[key] && !m_previousKeys[key]; }
    static inline bool isKeyDown(int key) { return m_currentKeys[key]; }
    static inline bool isKeyUp(int key) { return !m_currentKeys[key] && m_previousKeys[key]; }

    static inline bool isMouseButtonDown(int button) { return m_currentMouseButtons[button]; }
    static inline bool isMouseButtonPressed(int button) { return m_currentMouseButtons[button] && !m_previousMouseButtons[button]; }
    static inline bool isMouseButtonUp(int button) { return !m_currentMouseButtons[button] && m_previousMouseButtons[button]; }

    static inline void setCursorMode(int mode) { glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, mode); }
    static inline int getCursorMode() { return glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR); }
    static inline bool isGameMode() { return getCursorMode() == GLFW_CURSOR_DISABLED; }

    static inline void setUiCapturing(bool capturing) { m_uiCapturing = capturing; }
    static inline bool isUiCapturing() { return m_uiCapturing; }

    static inline void setGameViewFocused(bool focused) { m_gameViewFocused = focused; }
    static inline bool isGameViewFocused() { return m_gameViewFocused; }

private:
    static glm::vec2 m_currentMousePosition;
    static glm::vec2 m_previousMousePosition;
    static bool m_currentKeys[GLFW_KEY_LAST];
    static bool m_previousKeys[GLFW_KEY_LAST];
    static bool m_currentMouseButtons[GLFW_MOUSE_BUTTON_LAST];
    static bool m_previousMouseButtons[GLFW_MOUSE_BUTTON_LAST];
    static bool m_uiCapturing;
    static bool m_gameViewFocused;
};
    