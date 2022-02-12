#pragma once

#include "Engine/Vector.hpp"
#include "Engine/Graphics/Color.hpp"

#include <vector>

namespace Engine
{
namespace ECS
{
/**
 * Component struct containing data about a line
 */
struct LineComponent
{
    /**
     * Points in the line
     */
    std::vector<Vector2f> points;

    /**
     * Line color
     */
    Color color;

    /**
     * Line thickness
     */
    float thickness;
};
}
}
