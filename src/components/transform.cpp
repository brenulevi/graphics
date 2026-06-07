#include "transform.h"

Transform::Transform(
    GameObject* owner,
    const glm::vec3& position,
    const glm::vec3& rotation,
    const glm::vec3& scale
)
    : Component(owner),
    m_position(position),
    m_rotation(rotation),
    m_scale(scale)
{
}

glm::vec3 Transform::getForward() const
{
    return glm::vec3(
        cos(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x)),
        sin(glm::radians(m_rotation.x)),
        sin(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x))
    );
}

glm::vec3 Transform::getRight() const
{
    return glm::normalize(glm::cross(getForward(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Transform::getUp() const
{
    return glm::normalize(glm::cross(getRight(), getForward()));
}

glm::mat4 Transform::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, m_scale);
    return model;
}

glm::mat4 Transform::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + getForward(), getUp());
}
