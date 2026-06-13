#include "playercontroller.h"

PlayerController::PlayerController(GameObject *owner, float moveSpeed, float sensitivity)
    : Component(owner), m_moveSpeed(moveSpeed), m_sensitivity(sensitivity)
{
}

void PlayerController::start()
{
    auto transform = getGameObject()->getComponent<Transform>();
    if (!transform)
        throw std::runtime_error("PlayerController requires a Transform component");

    m_firstMouse = true;
    syncRotationFromTransform();
}

void PlayerController::update(float deltaTime)
{
    auto transform = getGameObject()->getComponent<Transform>();
    if (!transform)
        throw std::runtime_error("PlayerController requires a Transform component");

    glm::vec3 direction(0.0f);
    if (Input::isKeyDown(GLFW_KEY_W))
        direction += transform->getForward();
    if (Input::isKeyDown(GLFW_KEY_S))
        direction -= transform->getForward();
    if (Input::isKeyDown(GLFW_KEY_A))
        direction -= transform->getRight();
    if (Input::isKeyDown(GLFW_KEY_D))
        direction += transform->getRight();
    if (Input::isKeyDown(GLFW_KEY_SPACE))
        direction += glm::vec3(0.0f, 1.0f, 0.0f);
    if (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL))
        direction -= glm::vec3(0.0f, 1.0f, 0.0f);

    if (glm::length(direction) > 0.0f)
    {
        direction = glm::normalize(direction);
        transform->translate(direction * m_moveSpeed * deltaTime);
    }

    if (Input::getCursorMode() != GLFW_CURSOR_DISABLED)
    {
        m_firstMouse = true;
        return;
    }

    glm::vec2 mouseDelta = Input::getMouseDelta();
    if (m_firstMouse)
    {
        mouseDelta = glm::vec2(0.0f);
        m_firstMouse = false;
    }

    m_yaw -= mouseDelta.x * m_sensitivity;
    m_pitch -= mouseDelta.y * m_sensitivity;

    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
    m_yaw = std::fmod(m_yaw, 360.0f);

    transform->setLocalRotationEuler(glm::vec3(m_pitch, m_yaw, 0.0f));
}

void PlayerController::syncRotationFromTransform()
{
    auto transform = getGameObject()->getComponent<Transform>();
    if (!transform)
        return;

    auto localRotation = transform->getLocalRotationEuler();
    m_yaw = localRotation.y;
    m_pitch = localRotation.x;
}
