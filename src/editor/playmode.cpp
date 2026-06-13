#include "playmode.h"

void PlayMode::start()
{
    if (m_isPlaying)
        return;

    m_isPlaying = true;
    m_playJustStarted = true;
}

void PlayMode::stop()
{
    if (!m_isPlaying)
        return;

    m_isPlaying = false;
    Input::setCursorMode(GLFW_CURSOR_NORMAL);
}

void PlayMode::handleInput()
{
    if (!m_isPlaying)
        return;

    if (Input::isKeyPressed(GLFW_KEY_ESCAPE))
        stop();
}

bool PlayMode::consumePlayStarted()
{
    if (!m_playJustStarted)
        return false;

    m_playJustStarted = false;
    return true;
}
