#pragma once

#include "Engine/ECS/ECS.hpp"

#include "Engine/Graphics/Renderer.hpp"

namespace Engine
{
namespace ECS
{
/**
 * System for rendering lines
 */
class LineRendererSystem
{
private:

public:
    /**
     * @brief Constructor
     */
    LineRendererSystem();

    /**
     * @brief Destructor
     */
    ~LineRendererSystem();

    /**
     * @brief Draws all the entities that have the appropriate components.
     * @param[in] renderer Renderer
     */
    void Draw(Renderer& renderer);
};
}
}
