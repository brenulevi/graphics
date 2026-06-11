#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char *path)
{
    unsigned char *data = loadTextureFromFile(path);

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::Texture(int width, int height, TextureFormat format)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum internalFormat;
    GLenum dataFormat;

    switch (format)
    {
        case TextureFormat::RGB:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            break;
        case TextureFormat::RGBA:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
        case TextureFormat::Depth:
            internalFormat = GL_DEPTH_COMPONENT24;
            dataFormat = GL_DEPTH_COMPONENT;
            break;
        default:
            throw std::runtime_error("Unsupported texture format");
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, nullptr);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

unsigned char *Texture::loadTextureFromFile(const char *path)
{
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(path, &m_width, &m_height, &m_channels, 4);
    if(!data)
    {
        throw std::runtime_error("Failed to load texture: " + std::string(path));
    }

    return data;
}
