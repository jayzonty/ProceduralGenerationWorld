#pragma once

#include "Engine/Vector.hpp"

namespace Engine
{
/**
 * Circle struct
 */
struct Circle
{
    /**
     * Circle center
     */
    Vector2f center;

    /**
     * Circle radius
     */
    float radius;

    /**
     * @brief Checks whether the specified point is inside this
     * circle or not.
     * @param[in] point Point to check
     * @return Returns true if the specified point is inside this circle.
     * Returns false otherwise.
     */
    bool IsPointInside(const Vector2f& point) const
    {
        Vector2f v = point - center;
        return v.GetSquaredMagnitude() < (radius * radius);
    }
};
}
