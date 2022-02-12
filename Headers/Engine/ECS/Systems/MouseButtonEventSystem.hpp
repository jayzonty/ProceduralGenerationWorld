#pragma once

#include "Engine/ECS/ECS.hpp"

namespace Engine
{
namespace ECS
{
/**
 * Class for an ECS system for handling mouse button events
 */
class MouseButtonEventSystem
{
public:
    /**
     * @brief Constructor
     */
    MouseButtonEventSystem();

    /**
     * @brief Destructor
     */
    ~MouseButtonEventSystem();

    /**
     * @brief Updates the system.
     * @param[in] deltaTime Time elapsed since the previous frame
     */
    void Update(float deltaTime);
};
}
}
