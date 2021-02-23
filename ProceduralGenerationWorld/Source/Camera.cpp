#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

/// <summary>
/// Constructor
/// </summary>
Camera::Camera()
	: m_fov(90.0f)
	, m_aspectRatio(1.0f)
	, m_position(0.0f)
	, m_yaw(0.0f)
	, m_pitch(0.0f)
	, m_forward(0.0f, 0.0f, -1.0f)
	, m_right(1.0f, 0.0f, 0.0f)
	, m_up(0.0f, 1.0f, 0.0f)
	, m_worldUp(0.0f, 1.0f, 0.0f)
{
	UpdateVectors();
}

/// <summary>
/// Destructor
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// Sets the camera's field of view
/// </summary>
/// <param name="fov">New field of view in degrees</param>
void Camera::SetFieldOfView(const float& fov)
{
	m_fov = fov;
}

/// <summary>
/// Gets the camera's field of view
/// </summary>
/// <returns>Field of view in degrees</returns>
float Camera::GetFieldOfView() const
{
	return m_fov;
}

/// <summary>
/// Sets the camera's aspect ratio
/// </summary>
/// <param name="aspectRatio">New aspect ratio</param>
void Camera::SetAspectRatio(const float& aspectRatio)
{
	m_aspectRatio = aspectRatio;
}

/// <summary>
/// Gets the camera's aspect ratio
/// </summary>
/// <returns>Aspect ratio</returns>
float Camera::GetAspectRatio() const
{
	return m_aspectRatio;
}

/// <summary>
/// Sets the camera's position
/// </summary>
/// <param name="position">New position</param>
void Camera::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

/// <summary>
/// Gets the camera's position
/// </summary>
/// <returns>Camera's position</returns>
glm::vec3 Camera::GetPosition() const
{
	return m_position;
}

/// <summary>
/// Sets the camera's yaw
/// </summary>
/// <param name="yaw">New yaw value</param>
void Camera::SetYaw(float yaw)
{
	m_yaw = yaw;
	UpdateVectors();
}

/// <summary>
/// Gets the camera's yaw
/// </summary>
/// <returns>Camera yaw</returns>
float Camera::GetYaw() const
{
	return m_yaw;
}

/// <summary>
/// Sets the camera's pitch
/// </summary>
/// <param name="pitch">New pitch value</param>
void Camera::SetPitch(float pitch)
{
	m_pitch = pitch;
	UpdateVectors();
}

/// <summary>
/// Gets the camera's pitch
/// </summary>
/// <returns>Camera pitch</returns>
float Camera::GetPitch() const
{
	return m_pitch;
}

/// <summary>
/// Gets the forward vector
/// </summary>
/// <returns>Camera's forward vector</returns>
glm::vec3 Camera::GetForwardVector() const
{
	return m_forward;
}

/// <summary>
/// Gets the right vector
/// </summary>
/// <returns>Camera's right vector</returns>
glm::vec3 Camera::GetRightVector() const
{
	return m_right;
}

/// <summary>
/// Gets the up vector
/// </summary>
/// <returns></returns>
glm::vec3 Camera::GetUpVector() const
{
	return m_up;
}

/// <summary>
/// Sets the world's up vector
/// </summary>
/// <param name="worldUp">World up vector</param>
void Camera::SetWorldUpVector(const glm::vec3& worldUp)
{
	m_worldUp = worldUp;
	UpdateVectors();
}

/// <summary>
/// Gets the world's up vector
/// </summary>
/// <returns>World up vector</returns>
glm::vec3 Camera::GetWorldUpVector() const
{
	return m_worldUp;
}

/// <summary>
/// Gets the view matrix
/// </summary>
/// <returns>View matrix</returns>
glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

/// <summary>
/// Gets the projection matrix
/// </summary>
/// <returns>Projection matrix</returns>
glm::mat4 Camera::GetProjectionMatrix() const
{
	// TODO: Make the near and far values adjustable
	return glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.1f, 100.0f);
}

/// <summary>
/// Updates the forward, right, and up vectors
/// </summary>
void Camera::UpdateVectors()
{
	m_forward.x = glm::cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_forward.y = glm::sin(glm::radians(m_pitch));
	m_forward.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	m_forward = glm::normalize(m_forward);

	m_right = glm::cross(m_forward, m_worldUp);

	m_up = glm::cross(m_right, m_forward);
}
