#include "Camera.h"

Camera::Camera(const glm::vec3& position, float fov, float aspectRatio, float near, float far) 
    :
    m_Position(position), m_Fov(fov), m_AspectRatio(aspectRatio), m_Near(near), m_Far(far), m_Yaw(0), m_Pitch(0)
{
    RecalculateVectors();
    RecalculateViewMatrix();
    RecalculateProjectionMatrix();
}

const glm::mat4& Camera::GetViewMatrix() const 
{
    return m_ViewMatrix; 
}

const glm::mat4& Camera::GetProjectionMatrix() const 
{
    return m_ProjectionMatrix; 
}

const glm::vec3& Camera::GetPosition() const 
{
    return m_Position; 
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    RecalculateViewMatrix();
}

void Camera::SetSpeed(float speed)
{
    m_Speed = speed;
}

void Camera::Move(Direction direction, float dt)
{
    switch (direction)
    {
    case Camera::Direction::Up:
        m_Position += m_Speed * dt * DEFAULT_UP;
        break;
    case Camera::Direction::Down:
        m_Position -= m_Speed * dt * DEFAULT_UP;
        break;
    case Camera::Direction::Front:
        m_Position += m_Speed * dt * m_Front;
        break;
    case Camera::Direction::Back:
        m_Position -= m_Speed * dt * m_Front;
        break;
    case Camera::Direction::Right:
        m_Position += m_Speed * dt * m_Right;
        break;
    case Camera::Direction::Left:
        m_Position -= m_Speed * dt * m_Right;
        break;
    }

    RecalculateViewMatrix();
}

void Camera::Rotate(float angle, const glm::vec3& axis)
{
    m_Rotation = glm::rotate(m_Rotation, angle, axis);
    RecalculateVectors();
    RecalculateViewMatrix();
}

void Camera::ProcessMouseInput(const glm::vec2& mouseOffset)
{
    m_Yaw -= mouseOffset.x;
    m_Pitch -= mouseOffset.y;

    if (m_Pitch > 89.0f) m_Pitch = 89.0f;
    else if (m_Pitch < -89.0f) m_Pitch = -89.0f;

    m_Rotation = glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_Yaw), glm::vec3(0, 1, 0));
    m_Rotation = glm::rotate(m_Rotation, glm::radians(m_Pitch), glm::vec3(1, 0, 0));

    RecalculateVectors();
    RecalculateViewMatrix();
}

void Camera::SetFOV(float fov)
{
    m_Fov = fov;
    RecalculateProjectionMatrix();
}

void Camera::SetAspectRatio(float aspectRatio)
{
    m_AspectRatio = aspectRatio;
    RecalculateProjectionMatrix();
}

void Camera::SetNear(float near)
{
    m_Near = near;
    RecalculateProjectionMatrix();
}

void Camera::SetFar(float far)
{
    m_Far = far;
    RecalculateProjectionMatrix();
}

void Camera::RecalculateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::RecalculateProjectionMatrix()
{
    m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);
}

void Camera::RecalculateVectors()
{
    m_Right = glm::vec3(m_Rotation * glm::vec4(DEFAULT_RIGHT, 1.0f));
    m_Front = glm::vec3(m_Rotation * glm::vec4(DEFAULT_FRONT, 1.0f));
    m_Up    = glm::vec3(m_Rotation * glm::vec4(DEFAULT_UP, 1.0f));
}
