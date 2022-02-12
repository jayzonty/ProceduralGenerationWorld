#pragma once

#include "Engine/ECS/ECS.hpp"
#include "Engine/Graphics/Renderer.hpp"

#include <memory>

namespace Engine
{
namespace ECS
{
/**
 * ECS system responsible for rendering shapes
 */
class ShapeRendererSystem
{
private:

public:
    /**
     * @brief Constructor
     */
    ShapeRendererSystem();

    /**
     * @brief Destructor
     */
    ~ShapeRendererSystem();

    /**
     * @brief Initializes the system.
     */
    void Init();

    /**
     * @brief Draws all the entities that have the appropriate components.
     * @param[in] renderer Renderer
     */
    void Draw(Renderer& renderer);

    /**
     * @brief Cleans up the system.
     */
    void Cleanup();
};
}
}
