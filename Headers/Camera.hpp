#pragma once

#include <glm/glm.hpp>

/// <summary>
/// Camera class
/// </summary>
class Camera
{
private:
	/// <summary>
	/// Vertical field of view
	/// </summary>
	float m_fov;

	/// <summary>
	/// Aspect ratio
	/// </summary>
	float m_aspectRatio;

	/// <summary>
	/// Camera position
	/// </summary>
	glm::vec3 m_position;

	/// <summary>
	/// Yaw angle
	/// </summary>
	float m_yaw;

	/// <summary>
	/// Pitch angle
	/// </summary>
	float m_pitch;

	/// <summary>
	/// Camera look direction
	/// </summary>
	glm::vec3 m_forward;

	/// <summary>
	/// Right vector
	/// </summary>
	glm::vec3 m_right;

	/// <summary>
	/// Up vector
	/// </summary>
	glm::vec3 m_up;

	/// <summary>
	/// World up vector
	/// </summary>
	glm::vec3 m_worldUp;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	Camera();

	/// <summary>
	/// Destructor
	/// </summary>
	~Camera();

	/// <summary>
	/// Sets the camera's field of view
	/// </summary>
	/// <param name="fov">New field of view in degrees</param>
	void SetFieldOfView(const float& fov);

	/// <summary>
	/// Gets the camera's field of view
	/// </summary>
	/// <returns>Field of view in degrees</returns>
	float GetFieldOfView() const;

	/// <summary>
	/// Sets the camera's aspect ratio
	/// </summary>
	/// <param name="aspectRatio">New aspect ratio</param>
	void SetAspectRatio(const float& aspectRatio);

	/// <summary>
	/// Gets the camera's aspect ratio
	/// </summary>
	/// <returns>Aspect ratio</returns>
	float GetAspectRatio() const;

	/// <summary>
	/// Sets the camera's position
	/// </summary>
	/// <param name="position">New position</param>
	void SetPosition(const glm::vec3& position);

	/// <summary>
	/// Gets the camera's position
	/// </summary>
	/// <returns>Camera's position</returns>
	glm::vec3 GetPosition() const;

	/// <summary>
	/// Sets the camera's yaw
	/// </summary>
	/// <param name="yaw">New yaw value</param>
	void SetYaw(float yaw);

	/// <summary>
	/// Gets the camera's yaw
	/// </summary>
	/// <returns>Camera yaw in degrees</returns>
	float GetYaw() const;

	/// <summary>
	/// Sets the camera's pitch
	/// </summary>
	/// <param name="pitch">New pitch value in degrees</param>
	void SetPitch(float pitch);

	/// <summary>
	/// Gets the camera's pitch
	/// </summary>
	/// <returns>Camera pitch in degrees</returns>
	float GetPitch() const;

	/// <summary>
	/// Gets the forward vector
	/// </summary>
	/// <returns>Camera's forward vector</returns>
	glm::vec3 GetForwardVector() const;

	/// <summary>
	/// Gets the right vector
	/// </summary>
	/// <returns>Camera's right vector</returns>
	glm::vec3 GetRightVector() const;

	/// <summary>
	/// Gets the up vector
	/// </summary>
	/// <returns></returns>
	glm::vec3 GetUpVector() const;

	/// <summary>
	/// Sets the world's up vector
	/// </summary>
	/// <param name="worldUp">World up vector</param>
	void SetWorldUpVector(const glm::vec3& worldUp);

	/// <summary>
	/// Gets the world's up vector
	/// </summary>
	/// <returns>World up vector</returns>
	glm::vec3 GetWorldUpVector() const;

	/// <summary>
	/// Gets the view matrix
	/// </summary>
	/// <returns>View matrix</returns>
	glm::mat4 GetViewMatrix() const;

	/// <summary>
	/// Gets the projection matrix
	/// </summary>
	/// <returns>Projection matrix</returns>
	glm::mat4 GetProjectionMatrix() const;

private:
	/// <summary>
	/// Updates the forward, right, and up vectors
	/// </summary>
	void UpdateVectors();
};
