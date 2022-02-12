#pragma once

#include <glm/glm.hpp>

/**
 * Struct containing data about a ray
 */
struct Ray
{
public:
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
	 * @param[in] newOrigin New origin
	 * @param[in] newDirection New direction
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
