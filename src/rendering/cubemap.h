#pragma once

#include <glad/gl.h>
#include <string>
#include <vector>
#include <stdexcept>

struct CubeMapFace {
  unsigned char *pixels;
  int width;
  int height;
  int channels;
};

class Cubemap {
public:
  Cubemap(const std::vector<std::string> &faces);
  ~Cubemap();

  void bind(unsigned int slot = 0) const;

  inline unsigned int getID() const { return m_id; }

private:
  CubeMapFace loadFace(const std::string &path);

private:
  unsigned int m_id;
};
