#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera 
{
public:
    enum class Direction
    {
        Up = 0,
        Down,
        Front,
        Back,
        Right,
        Left,
    };

public:
    Camera(const glm::vec3& position, float fov, float aspectRatio, float near, float far);

    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetRotationMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;
    const glm::vec3& GetPosition() const;

    void SetPosition(const glm::vec3& position);
    void SetAspectRatio(float aspectRatio);
    void SetSpeed(float speed);
    void SetNear(float near);
    void SetFar(float far);
    void SetFOV(float fov);

    void ProcessMouseInput(const glm::vec2& mouseOffset);
    void Rotate(float angle, const glm::vec3& axis);
    void Move(Direction direction, float dt);

private:
    void RecalculateViewMatrix();
    void RecalculateProjectionMatrix();
    void RecalculateVectors();

private:
    glm::mat4 m_Rotation = glm::mat4(1.0f);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

    glm::vec3 m_Position;
    glm::vec3 m_Front = DEFAULT_FRONT;
    glm::vec3 m_Right = DEFAULT_RIGHT;
    glm::vec3 m_Up = DEFAULT_UP;

    float m_Speed = 100.0f;
    float m_Fov;
    float m_AspectRatio;
    float m_Near;
    float m_Far;
    float m_Yaw;
    float m_Pitch;
    float m_Roll;

    const glm::vec3 DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 DEFAULT_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);
};
