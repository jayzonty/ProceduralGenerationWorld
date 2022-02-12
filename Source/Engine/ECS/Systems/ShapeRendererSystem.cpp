#include "Engine/ECS/Systems/ShapeRendererSystem.hpp"

#include "Engine/ECS/Components/SceneComponent.hpp"
#include "Engine/ECS/Components/ShapeComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/ECS.hpp"
#include <iostream>
#include <memory>

namespace Engine
{
namespace ECS
{
/**
 * @brief Constructor
 */
ShapeRendererSystem::ShapeRendererSystem()
{
}

/**
 * @brief Destructor
 */
ShapeRendererSystem::~ShapeRendererSystem()
{
}

/**
 * @brief Initializes the system.
 */
void ShapeRendererSystem::Init()
{
}

/**
 * @brief Draws all the entities that have the ShapeComponent component.
 * @param[in] renderer Renderer
 */
void ShapeRendererSystem::Draw(Renderer& renderer)
{
    std::set<entity_t> entities = Registry::GetEntitiesWithComponents<TransformComponent, ShapeComponent>();
    //std::cout << "Num entities: " << entities.size() << std::endl;
    for (entity_t entity : entities)
    {
        ShapeComponent* shape = Registry::GetComponent<ShapeComponent>(entity);
        if (!shape->isVisible)
        {
            continue;
        }
        
        TransformComponent* transform = Registry::GetComponent<TransformComponent>(entity);

        Vector2f position = transform->localPosition;
        while (transform->parent != nullptr)
        {
            position = position + transform->parent->localPosition;
            transform = transform->parent;
        }

        int32_t order = transform->GetWorldDepth();
        if (shape->type == ShapeComponent::Type::Circle)
        {
            renderer.DrawCircle(position, shape->radius, shape->color, shape->renderLayer, order);
        }
        else if (shape->type == ShapeComponent::Type::Rectangle)
        {
            renderer.DrawQuad(position, shape->halfExtents, shape->color, shape->renderLayer, order);
        }
    }
}

/**
 * @brief Cleans up the system.
 */
void ShapeRendererSystem::Cleanup()
{
}
}
}
