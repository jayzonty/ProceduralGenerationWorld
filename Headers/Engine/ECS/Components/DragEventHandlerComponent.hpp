#pragma once

#include "Engine/Geometry/AABB.hpp"
#include "Engine/ECS/ECS.hpp"

#include <functional>

namespace Engine
{
namespace ECS
{
/**
 * Component struct containing data about handling a drag event
 */
struct DragEventHandlerComponent
{
    /**
     * Area where the drag event should register
     */
    AABB dragBounds;

    /**
     * Function to be called when the drag event is
     * started on the entity this component is attached to
     */
    std::function<void(entity_t)> dragStart;

    /**
     * Function to be called when the entity
     * this component is attached to is being dragged
     */
    std::function<void(entity_t)> dragging;

    /**
     * Function to be called when the drag event is
     * ended on the entity this component is attached to
     */
    std::function<void(entity_t)> dragEnd;

    /**
     * Function to be called when the drag event that started
     * from this entity ended on top of another entity that accepts
     * drop from a drag.
     * The first argument is the source entity (the entity that started the drag),
     * and the second argument is the destination entity.
     * The function returns a boolean indicating whether to consume the event or not.
     */
    std::function<bool(entity_t, entity_t)> dragDropped;
};
}
}
