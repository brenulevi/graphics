#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "framebuffer.h"
#include "resources/assetmanager.h"
#include "texture.h"
#include "shader.h"

class ShadowMap {
public:
  ShadowMap(int width, int height);
  ~ShadowMap();

  void beginRender() const;
  void endRender() const;

  inline void setLightSpaceMatrix(const glm::mat4& lightSpaceMatrix) { m_lightSpaceMatrix = lightSpaceMatrix; }

  inline std::shared_ptr<Texture> getDepthTexture() const { return m_depthTexture; }
  inline const glm::mat4& getLightSpaceMatrix() const { return m_lightSpaceMatrix; }
  inline std::shared_ptr<Shader> getShadowShader() const { return m_shadowShader; }
  inline int getWidth() const { return m_width; }
  inline int getHeight() const { return m_height; }

private:
  std::shared_ptr<Framebuffer> m_framebuffer;
  std::shared_ptr<Texture> m_depthTexture;
  std::shared_ptr<Shader> m_shadowShader;
  int m_width;
  int m_height;
  glm::mat4 m_lightSpaceMatrix;
};