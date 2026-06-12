#pragma once

#include <stdexcept>
#include <glad/gl.h>

enum class TextureFormat
{
    RGB,
    RGBA,
    Depth
};

enum class TextureWrapMode
{
    Repeat,
    MirroredRepeat,
    ClampToEdge
};

enum class TextureFilterMode
{
    Nearest,
    Linear
};

class Texture
{
public:
    Texture(const char *path);
    Texture(int width, int height, TextureFormat format);
    ~Texture();

    void bind(unsigned int slot = 0) const;

    inline unsigned int getID() const { return m_id; }
    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }
    inline int getChannels() const { return m_channels; }

    void setWrapMode(TextureWrapMode wrapModeS, TextureWrapMode wrapModeT);
    void setFilterMode(TextureFilterMode filterModeMin, TextureFilterMode filterModeMag);

private:
    unsigned char *loadTextureFromFile(const char *path);

private:
    unsigned int m_id;
    int m_width, m_height, m_channels;
};
