#pragma once

#include "Engine/Vector.hpp"

namespace Engine
{
/**
 * Axis-aligned bounding box
 */
struct AABB
{
    /**
     * AABB center
     */
    Vector2f center;

    /**
     * AABB half-extents
     */
    Vector2f halfExtents;

    /**
     * @brief Gets the minimum point of this AABB.
     * @return Minimum point of this AABB
     */
    Vector2f GetMin() const
    {
        return center - halfExtents;
    }

    /**
     * @brief Gets the maximum point of this AABB.
     * @return Maximum point of this AABB
     */
    Vector2f GetMax() const
    {
        return center + halfExtents;
    }
    
    /**
     * @brief Checks whether the specified point is inside this AABB.
     * @param[in] x X-coordinate of the point
     * @param[in] y Y-coordinate of the point
     * @return Returns true if the specified point is inside the AABB. Returns false otherwise.
     */
    bool ContainsPoint(float x, float y) const
    {
        Vector2f min = GetMin(), max = GetMax();
        return (min.x() <= x && x <= max.x()) && (min.y() <= y && y <= max.y());
    }

    /**
     * @brief Checks whether the specified point is inside this AABB.
     * @param[in] point The point
     * @return Returns true if the specified point is inside the AABB. Returns false otherwise.
     */
    bool ContainsPoint(const Vector2f& point) const
    {
        return ContainsPoint(point.x(), point.y());
    }

    /**
     * @brief Creates an AABB with the specified minimum and maximum points.
     * @param[in] min Minimum point
     * @param[in] max Maximum point
     * @return Created AABB
     */
    static AABB FromMinMax(const Vector2f& min, const Vector2f& max)
    {
        AABB ret;
        ret.center = (min + max) / 2.0f;
        ret.halfExtents = (max - min) / 2.0f;
        return ret;
    }
};
}
