#include "renderer.h"
#include "glad/gl.h"

Renderer::Renderer() : m_viewportSize(800.0f, 600.0f) {
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD!");
  }

  setupSkybox();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void Renderer::clear() const {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::beginScene(const SceneData &sceneData) {
  m_sceneData = sceneData;

  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);

  setViewportSize(m_viewportSize.x, m_viewportSize.y);
}

void Renderer::draw(const Transform &transform, const Mesh &mesh,
                    const Material &material) {
  auto shader = material.getShader();
  auto diffuse = material.getDiffuse();
  auto specular = material.getSpecular();

  if (!shader) {
    throw std::runtime_error("Material has no shader!");
  }

  shader->use();

  if (diffuse) {
    diffuse->bind(0);
    shader->setInt("u_Material.diffuse", 0);
  }

  if (specular) {
    specular->bind(1);
    shader->setInt("u_Material.specular", 1);
  }

  if (m_shadowMap) {
    m_shadowMap->getDepthTexture()->bind(5);
    shader->setInt("u_ShadowMap", 5);
    shader->setMat4("u_LightSpaceMatrix", m_shadowMap->getLightSpaceMatrix());
  }

  shader->setFloat("u_Material.shininess", material.getShininess());

  shader->setVec3("u_ViewPos", m_sceneData.cameraPosition);

  shader->setVec3("u_DirectionalLight.direction",
                  m_sceneData.directionalLight.direction);
  shader->setVec3("u_DirectionalLight.color",
                  m_sceneData.directionalLight.color);
  shader->setFloat("u_DirectionalLight.intensity",
                   m_sceneData.directionalLight.intensity);

  shader->setInt("u_NumPointLights",
                 std::min(static_cast<int>(m_sceneData.pointLights.size()),
                          MAX_POINT_LIGHTS));
  for (size_t i = 0; i < m_sceneData.pointLights.size() && i < MAX_POINT_LIGHTS;
       ++i) {
    const auto &pointLight = m_sceneData.pointLights[i];
    shader->setVec3("u_PointLights[" + std::to_string(i) + "].position",
                    pointLight.position);
    shader->setVec3("u_PointLights[" + std::to_string(i) + "].color",
                    pointLight.color);
    shader->setFloat("u_PointLights[" + std::to_string(i) + "].intensity",
                     pointLight.intensity);
    shader->setFloat("u_PointLights[" + std::to_string(i) + "].constant",
                     pointLight.constant);
    shader->setFloat("u_PointLights[" + std::to_string(i) + "].linear",
                     pointLight.linear);
    shader->setFloat("u_PointLights[" + std::to_string(i) + "].quadratic",
                     pointLight.quadratic);
  }

  shader->setInt("u_NumSpotLights",
                 std::min(static_cast<int>(m_sceneData.spotLights.size()),
                          MAX_SPOT_LIGHTS));
  for (size_t i = 0; i < m_sceneData.spotLights.size() && i < MAX_SPOT_LIGHTS;
       ++i) {
    const auto &spotLight = m_sceneData.spotLights[i];
    shader->setVec3("u_SpotLights[" + std::to_string(i) + "].position",
                    spotLight.position);
    shader->setVec3("u_SpotLights[" + std::to_string(i) + "].direction",
                    spotLight.direction);
    shader->setVec3("u_SpotLights[" + std::to_string(i) + "].color",
                    spotLight.color);
    shader->setFloat("u_SpotLights[" + std::to_string(i) + "].intensity",
                     spotLight.intensity);
    shader->setFloat("u_SpotLights[" + std::to_string(i) + "].constant",
                     spotLight.constant);
    shader->setFloat("u_SpotLights[" + std::to_string(i) + "].linear",
                     spotLight.linear);
    shader->setFloat("u_SpotLights[" + std::to_string(i) + "].quadratic",
                     spotLight.quadratic);
    shader->setFloat("u_SpotLights[" + std::to_string(i) + "].innerCutoff",
                     spotLight.innerCutoff);
    shader->setFloat("u_SpotLights[" + std::to_string(i) + "].outerCutoff",
                     spotLight.outerCutoff);
  }

  auto viewProjectionMatrix = m_sceneData.projectionMatrix * m_sceneData.viewMatrix;
  shader->setMat4("u_ViewProjection", viewProjectionMatrix);
  shader->setMat4("u_Model", transform.getWorldMatrix());

  mesh.bind();
  glDrawElements(GL_TRIANGLES, mesh.getIndexBuffer().getCount(),
                 GL_UNSIGNED_INT, nullptr);
}

void Renderer::endScene() {}

void Renderer::beginShadowPass(ShadowMap *shadowMap) {
  m_shadowMap = shadowMap;
  m_shadowMap->beginRender();
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_FRONT);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawShadow(const Transform &transform, const Mesh &mesh) {
  auto shader = m_shadowMap->getShadowShader();
  shader->use();

  shader->setMat4("u_LightSpaceMatrix", m_shadowMap->getLightSpaceMatrix());
  shader->setMat4("u_Model", transform.getWorldMatrix());

  mesh.bind();
  glDrawElements(GL_TRIANGLES, mesh.getIndexBuffer().getCount(),
                 GL_UNSIGNED_INT, nullptr);
}

void Renderer::endShadowPass() { m_shadowMap->endRender(); }

void Renderer::drawSkybox(const Skybox &skybox) {
  glDepthMask(GL_FALSE);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);

  auto shader = m_skyboxShader;

  if (!shader) {
    throw std::runtime_error("Skybox shader not found!");
  }

  shader->use();

  auto viewProjectionMatrix = m_sceneData.projectionMatrix * glm::mat4(glm::mat3(m_sceneData.viewMatrix));
  shader->setMat4("u_ViewProjection", viewProjectionMatrix);

  skybox.getCubemap()->bind(0);
  shader->setInt("u_Cubemap", 0);

  m_skyboxMesh->bind();
  glDrawElements(GL_TRIANGLES, m_skyboxMesh->getIndexBuffer().getCount(),
                 GL_UNSIGNED_INT, nullptr);

  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
}

void Renderer::setViewportSize(float width, float height) {
  m_viewportSize = {width, height};

  glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
}

void Renderer::setupSkybox() {
  m_skyboxShader = AssetManager::loadShader(
      "skybox", "assets/shaders/skybox.vert", "assets/shaders/skybox.frag");

  std::vector<Vertex> vertices = {
      {{-1.0f, 1.0f, -1.0f}, {}, {}},
      {{-1.0f, -1.0f, -1.0f}, {}, {}},
      {{1.0f, -1.0f, -1.0f}, {}, {}},
      {{1.0f, -1.0f, -1.0f}, {}, {}},
      {{1.0f, 1.0f, -1.0f}, {}, {}},
      {{-1.0f, 1.0f, -1.0f}, {}, {}},

      {{-1.0f, -1.0f, 1.0f}, {}, {}},
      {{-1.0f, -1.0f, -1.0f}, {}, {}},
      {{-1.0f, 1.0f, -1.0f}, {}, {}},
      {{-1.0f, 1.0f, -1.0f}, {}, {}},
      {{-1.0f, 1.0f, 1.0f}, {}, {}},
      {{-1.0f, -1.0f, 1.0f}, {}, {}},

      {{1.0f, -1.0f, -1.0f}, {}, {}},
      {{1.0f, -1.0f, 1.0f}, {}, {}},
      {{1.0f, 1.0f, 1.0f}, {}, {}},
      {{1.0f, 1.0f, 1.0f}, {}, {}},
      {{1.0f, 1.0f, -1.0f}, {}, {}},
      {{1.0f, -1.0f, -1.0f}, {}, {}},

      {{-1.0f, -1.0f, 1.0f}, {}, {}},
      {{-1.0f, 1.0f, 1.0f}, {}, {}},
      {{1.0f, 1.0f, 1.0f}, {}, {}},
      {{1.0f, 1.0f, 1.0f}, {}, {}},
      {{1.0f, -1.0f, 1.0f}, {}, {}},
      {{-1.0f, -1.0f, 1.0f}, {}, {}},

      {{-1.0f, 1.0f, -1.0f}, {}, {}},
      {{1.0f, 1.0f, -1.0f}, {}, {}},
      {{1.0f, 1.0f, 1.0f}, {}, {}},
      {{1.0f, 1.0f, 1.0f}, {}, {}},
      {{-1.0f, 1.0f, 1.0f}, {}, {}},
      {{-1.0f, 1.0f, -1.0f}, {}, {}},

      {{-1.0f, -1.0f, -1.0f}, {}, {}},
      {{-1.0f, -1.0f, 1.0f}, {}, {}},
      {{1.0f, -1.0f, -1.0f}, {}, {}},
      {{1.0f, -1.0f, -1.0f}, {}, {}},
      {{-1.0f, -1.0f, 1.0f}, {}, {}},
      {{1.0f, -1.0f, 1.0f}, {}, {}},
  };

  std::vector<unsigned int> indices;
  for (int i = 0; i < vertices.size(); i += 3) {
    indices.push_back(i);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
    indices.push_back(i + 2);
    indices.push_back(i + 1);
    indices.push_back(i + 3);
  }

  m_skyboxMesh = AssetManager::createMesh("skybox", vertices, indices);
}