#include "Engine/ECS/Systems/LineRendererSystem.hpp"

#include "Engine/ECS/Components/LineComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include <set>

namespace Engine
{
namespace ECS
{
/**
 * @brief Constructor
 */
LineRendererSystem::LineRendererSystem()
{
}

/**
 * @brief Destructor
 */
LineRendererSystem::~LineRendererSystem()
{
}

/**
 * @brief Draws all the entities that have the appropriate components.
 * @param[in] renderer Renderer
 */
void LineRendererSystem::Draw(Renderer &renderer)
{
    std::set<entity_t> entities = Registry::GetEntitiesWithComponents<LineComponent>();
    for (entity_t entity : entities)
    {
        LineComponent* lineComponent = Registry::GetComponent<LineComponent>(entity);

        for (size_t i = 1; i < lineComponent->points.size(); ++i)
        {
            renderer.DrawLineSegment(
                lineComponent->points[i - 1],
                lineComponent->points[i],
                lineComponent->thickness / 2.0f,
                lineComponent->color);
        }
    }
}
}
}