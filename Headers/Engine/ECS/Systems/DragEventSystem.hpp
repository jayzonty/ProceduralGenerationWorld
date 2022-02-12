#pragma once

#include "Engine/ECS/ECS.hpp"

namespace Engine
{
namespace ECS
{
/**
 * System for handling dragging related events
 */
class DragEventSystem
{
private:
    /**
     * State enum
     */
    enum class State
    {
        Idle,
        WaitingForDrag,
        Dragging
    };

private:
    /**
     * Current state
     */
    State m_currentState;

    /**
     * Entity that is currently being dragged
     */
    entity_t m_draggedEntity;

public:
    /**
     * @brief Constructor
     */
    DragEventSystem();

    /**
     * @brief Destructor
     */
    ~DragEventSystem();

    /**
     * @brief Updates the system
     * @param[in] deltaTime Time elapsed since the previous frame
     */
    void Update(float deltaTime);
};
}
}
