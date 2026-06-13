#pragma once

#include "input.h"

class PlayMode
{
public:
    bool isPlaying() const { return m_isPlaying; }

    void start();
    void stop();
    void handleInput();

    bool consumePlayStarted();

private:
    bool m_isPlaying = false;
    bool m_playJustStarted = false;
};
