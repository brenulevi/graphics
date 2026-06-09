#include "transform.h"

Transform::Transform(
    GameObject* owner,
    const glm::vec3& position,
    const glm::vec3& rotation,
    const glm::vec3& scale
)
    : Component(owner),
    m_localPosition(position),
    m_localRotation(glm::quat(glm::radians(rotation))),
    m_localScale(scale)
{
}

glm::vec3 Transform::getWorldPosition() const
{
    return glm::vec3(getWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

glm::quat Transform::getWorldRotation() const
{
    GameObject* parent = getGameObject()->getParent();

    if (!parent)
        return m_localRotation;

    Transform* parentTransform = parent->getComponent<Transform>();
    if (!parentTransform)
        throw std::runtime_error("Parent game object has no transform component.");

    return glm::normalize(parentTransform->getWorldRotation() * m_localRotation);
}

glm::vec3 Transform::getWorldScale() const
{
    GameObject* parent = getGameObject()->getParent();

    if (!parent)
        return m_localScale;

    Transform* parentTransform = parent->getComponent<Transform>();
    if (!parentTransform)
        throw std::runtime_error("Parent game object has no transform component.");

    return parentTransform->getWorldScale() * m_localScale;
}

void Transform::rotateEuler(const glm::vec3& euler)
{
    glm::quat deltaRotation = glm::quat(glm::radians(euler));
    m_localRotation = glm::normalize(deltaRotation * m_localRotation);
}

glm::vec3 Transform::getForward() const
{
    return glm::normalize(getWorldRotation() * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 Transform::getRight() const
{
    return glm::normalize(getWorldRotation() * glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 Transform::getUp() const
{
    return glm::normalize(getWorldRotation() * glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Transform::getLocalMatrix() const
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_localPosition);
    glm::mat4 rotation = glm::toMat4(m_localRotation);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_localScale);
    return translation * rotation * scale;
}

glm::mat4 Transform::getWorldMatrix() const
{
    GameObject* parent = getGameObject()->getParent();

    if(!parent)
        return getLocalMatrix();

    Transform* parentTransform = parent->getComponent<Transform>();
    if (!parentTransform)
        throw std::runtime_error("Parent game object has no transform component.");

    return parentTransform->getWorldMatrix() * getLocalMatrix();
}