#include "cubemap.h"
#include <stb_image.h>


Cubemap::Cubemap(const std::vector<std::string> &faces)
{
    if (faces.size() != 6)
    {
        throw std::runtime_error("Cubemap requires exactly 6 face textures");
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

    int width = 0;
    int height = 0;

    for (unsigned int i = 0; i < faces.size(); ++i)
    {
        CubeMapFace face = loadFace(faces[i]);

        if (i == 0)
        {
            width = face.width;
            height = face.height;
        }
        else if (face.width != width || face.height != height)
        {
            stbi_image_free(face.pixels);
            throw std::runtime_error("Cubemap faces must have the same dimensions");
        }

        GLenum format = face.channels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            format,
            face.width,
            face.height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            face.pixels);

        stbi_image_free(face.pixels);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

Cubemap::~Cubemap()
{
    glDeleteTextures(1, &m_id);
}

void Cubemap::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
}

CubeMapFace Cubemap::loadFace(const std::string &path)
{
    stbi_set_flip_vertically_on_load(false);

    CubeMapFace face{};
    face.pixels = stbi_load(path.c_str(), &face.width, &face.height, &face.channels, 0);
    if (!face.pixels)
    {
        throw std::runtime_error("Failed to load cubemap face: " + path);
    }

    return face;
}