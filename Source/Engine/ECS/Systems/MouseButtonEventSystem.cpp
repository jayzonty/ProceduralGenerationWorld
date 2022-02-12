#include "Engine/ECS/Systems/MouseButtonEventSystem.hpp"

#include "Engine/ECS/Components/MouseButtonEventHandlerComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/Vector.hpp"
#include <set>

namespace Engine
{
namespace ECS
{
/**
 * @brief Constructor
 */
MouseButtonEventSystem::MouseButtonEventSystem()
{
}

/**
 * @brief Destructor
 */
MouseButtonEventSystem::~MouseButtonEventSystem()
{
}

/**
 * @brief Updates the system.
 * @param[in] deltaTime Time elapsed since the previous frame
 */
void MouseButtonEventSystem::Update(float deltaTime)
{
    if (Input::IsMouseButtonPressed(Input::Button::LEFT_MOUSE)
        || Input::IsMouseButtonPressed(Input::Button::RIGHT_MOUSE))
    {
        Input::Button button = Input::Button::LEFT_MOUSE;
        if (Input::IsMouseButtonPressed(Input::Button::RIGHT_MOUSE))
        {
            button = Input::Button::RIGHT_MOUSE;
        }

        Vector2f mousePosition = Input::GetMousePosition();

        std::vector<entity_t> candidateClickedEntities;
        std::set<entity_t> entities = Registry::GetEntitiesWithComponents<TransformComponent, MouseButtonEventHandlerComponent>();
        for (entity_t entity : entities)
        {
            MouseButtonEventHandlerComponent* clickHandler = Registry::GetComponent<MouseButtonEventHandlerComponent>(entity);
            if (!clickHandler->isEnabled)
            {
                continue;
            }
            
            TransformComponent* transform = Registry::GetComponent<TransformComponent>(entity);

            AABB clickBounds = clickHandler->bounds;
            clickBounds.center = clickBounds.center + transform->GetWorldPosition();
            if (clickBounds.ContainsPoint(mousePosition)
                && clickHandler->onPressed != nullptr)
            {
                candidateClickedEntities.push_back(entity);
            }
        }

        std::sort(
            candidateClickedEntities.begin(), 
            candidateClickedEntities.end(),
            [&](const entity_t& a, const entity_t& b)->bool
            {
                TransformComponent* aTransform = Engine::ECS::Registry::GetComponent<Engine::ECS::TransformComponent>(a);
                TransformComponent* bTransform = Engine::ECS::Registry::GetComponent<Engine::ECS::TransformComponent>(b);
                return aTransform->GetWorldDepth() > bTransform->GetWorldDepth();
            });
        
        for (entity_t entity : candidateClickedEntities)
        {
            MouseButtonEventHandlerComponent* clickEventHandler = Registry::GetComponent<MouseButtonEventHandlerComponent>(entity);
            if (clickEventHandler->onPressed(entity, button))
            {
                break;
            }
        }
    }
}
}
}