#pragma once

#include <glm/glm.hpp>

/// <summary>
/// Axis-aligned bounding box class
/// </summary>
class AABB
{
public:
	/// <summary>
	/// Minimum point
	/// </summary>
	glm::vec3 min;

	/// <summary>
	/// Maximum point
	/// </summary>
	glm::vec3 max;

	/// <summary>
	/// Constructor
	/// </summary>
	AABB()
		: min(0.0f)
		, max(0.0f)
	{
	}

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="minPoint">Minimum point</param>
	/// <param name="maxPoint">Maximum point</param>
	AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
		: min(minPoint)
		, max(maxPoint)
	{
	}

	/// <summary>
	/// Destructor
	/// </summary>
	~AABB()
	{
	}
};
