#pragma once

#include "Engine/ECS/ECS.hpp"
#include "Engine/Geometry/AABB.hpp"

#include "Engine/Input.hpp"

#include <functional>

namespace Engine
{
namespace ECS
{
/**
 * Component struct containing data about handling a mouse button event
 */
struct MouseButtonEventHandlerComponent
{
    /**
     * Function to be called when a mouse button pressed event is
     * started on the entity this component is attached to
     */
    std::function<bool(const entity_t&, const Input::Button&)> onPressed;

    /**
     * Area for receiving mouse button events
     */
    AABB bounds;

    /**
     * Flag indicating whether this component is enabled or not
     */
    bool isEnabled;
};
}
}
