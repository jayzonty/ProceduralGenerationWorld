#pragma once

#include <glm/glm.hpp>

/**
 * Axis-aligned bounding box class
 */
class AABB
{
public:
	/**
	 * Minimum point
	 */
	glm::vec3 min;

	/**
	 * Maximum point
	 */
	glm::vec3 max;

	/**
	 * @brief Constructor
	 */
	AABB()
		: min(0.0f)
		, max(0.0f)
	{
	}

	/**
	 * @brief Constructor
	 * @param[in] minPoint Minimum point
	 * @param[in] maxPoint Maximum point
	 */
	AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
		: min(minPoint)
		, max(maxPoint)
	{
	}

	/**
	 * @brief Destructor
	 */
	~AABB()
	{
	}
};
