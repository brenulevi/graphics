#pragma once

#include <stdexcept>
#include <glad/gl.h>

class Texture
{
public:
    Texture(const char *path);
    ~Texture();

    void bind(unsigned int slot = 0) const;

private:
    unsigned char *loadTextureFromFile(const char *path);

private:
    unsigned int m_id;
    int m_width, m_height, m_channels;
};