#pragma once

#include <glm/glm.hpp>

/**
 * Ray class
 */
struct Ray
{
	/**
	 * Ray origin
	 */
	glm::vec3 origin;

	/**
	 * Ray direction
	 */
	glm::vec3 direction;

	/**
	 * @brief Constructor
	 * @param[in] newOrigin Origin
	 * @param[in] newDirection Direction
	 */
	Ray(const glm::vec3& newOrigin, const glm::vec3& newDirection)
		: origin(newOrigin)
		, direction(newDirection)
	{
	}

	/**
	 * @brief Destructor
	 */
	~Ray()
	{
	}
};
