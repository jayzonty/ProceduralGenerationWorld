#pragma once

#include "Engine/Graphics/Color.hpp"
#include "Engine/Vector.hpp"

namespace Engine
{
namespace ECS
{
/**
 * Component containing data about a shape.
 */
struct ShapeComponent
{
    /**
     * Shape type enum
     */
    enum class Type
    {
        Circle,
        Rectangle
    };

    /**
     * Shape type
     */
    Type type;

    /**
     * Shape color
     */
    Color color;

    /**
     * Circle radius (for a circle shape)
     */
    float radius;

    /**
     * Half-extents (for a rectangle shape)
     */
    Vector2f halfExtents;

    /**
     * Flag indicating whether this shape is visible or not
     */
    bool isVisible;

    /**
     * Render layer
     */
    uint32_t renderLayer = 0;
};
}
}
