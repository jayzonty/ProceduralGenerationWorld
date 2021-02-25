#pragma once

#include <glm/glm.hpp>

/// <summary>
/// Ray class
/// </summary>
class Ray
{
public:
	/// <summary>
	/// Ray origin
	/// </summary>
	glm::vec3 origin;

	/// <summary>
	/// Ray direction
	/// </summary>
	glm::vec3 direction;

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="newOrigin">Origin</param>
	/// <param name="newDirection">Direction</param>
	Ray(const glm::vec3& newOrigin, const glm::vec3& newDirection)
		: origin(newOrigin)
		, direction(newDirection)
	{
	}

	/// <summary>
	/// Destructor
	/// </summary>
	~Ray()
	{
	}
};
