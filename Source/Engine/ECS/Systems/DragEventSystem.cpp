#include "Engine/ECS/Systems/DragEventSystem.hpp"

#include "Engine/ECS/ECS.hpp"
#include "Engine/Input.hpp"

#include "Engine/ECS/Components/DragEventHandlerComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <set>
#include <vector>

namespace Engine
{
namespace ECS
{
/**
 * @brief Constructor
 */
DragEventSystem::DragEventSystem()
    : m_currentState(State::Idle)
    , m_draggedEntity()
{
}

/**
 * @brief Destructor
 */
DragEventSystem::~DragEventSystem()
{
}

/**
 * @brief Updates the system
 * @param[in] deltaTime Time elapsed since the previous frame
 */
void DragEventSystem::Update(float deltaTime)
{
    Vector2f mousePosition = Input::GetMousePosition();
    if (m_currentState == State::Idle)
    {    
        if (Input::IsMouseButtonPressed(Input::Button::LEFT_MOUSE))
        {
            bool found = false;
            float maxZ = std::numeric_limits<float>::lowest();

            std::set<entity_t> entities = Registry::GetEntitiesWithComponents<TransformComponent, DragEventHandlerComponent>();
            for (entity_t entity : entities)
            {
                DragEventHandlerComponent* dragEventHandler = Registry::GetComponent<DragEventHandlerComponent>(entity);
                if ((dragEventHandler->dragStart == nullptr) 
                    && (dragEventHandler->dragging == nullptr) 
                    && (dragEventHandler->dragEnd == nullptr))
                {
                    continue;
                }

                TransformComponent* transform = Registry::GetComponent<TransformComponent>(entity);

                AABB dragBounds = dragEventHandler->dragBounds;
                dragBounds.center = dragBounds.center + transform->GetWorldPosition();
                if (dragBounds.ContainsPoint(mousePosition))
                {
                    if (transform->GetWorldDepth() > maxZ)
                    {
                        m_draggedEntity = entity;
                        maxZ = transform->GetWorldDepth();
                        found = true;
                    }
                }
            }

            if (found)
            {
                m_currentState = State::WaitingForDrag;
            }
        }
    }
    else if (m_currentState == State::WaitingForDrag)
    {
        if (Input::IsMouseButtonDown(Input::Button::LEFT_MOUSE))
        {
            DragEventHandlerComponent* dragEventHandler = Registry::GetComponent<DragEventHandlerComponent>(m_draggedEntity);
            if (dragEventHandler->dragStart != nullptr)
            {
                dragEventHandler->dragStart(m_draggedEntity);
            }
            m_currentState = State::Dragging;
        }
        else
        {
            m_currentState = State::Idle;
        }
    }
    else if (m_currentState == State::Dragging)
    {
        if (Input::IsMouseButtonDown(Input::Button::LEFT_MOUSE))
        {
            DragEventHandlerComponent* dragEventHandler = Registry::GetComponent<DragEventHandlerComponent>(m_draggedEntity);
            if (dragEventHandler->dragging != nullptr)
            {
                dragEventHandler->dragging(m_draggedEntity);
            }
        }
        else
        {
            DragEventHandlerComponent* dragEventHandler = Registry::GetComponent<DragEventHandlerComponent>(m_draggedEntity);

            std::vector<entity_t> candidateDestinationEntities;
            std::set<entity_t> entities = Registry::GetEntitiesWithComponents<TransformComponent, DragEventHandlerComponent>();
            for (entity_t entity : entities)
            {
                DragEventHandlerComponent* dragEventHandler = Registry::GetComponent<DragEventHandlerComponent>(entity);
                TransformComponent* transform = Registry::GetComponent<TransformComponent>(entity);

                AABB dragBounds = dragEventHandler->dragBounds;
                dragBounds.center = dragBounds.center + transform->GetWorldPosition();

                if (dragBounds.ContainsPoint(mousePosition))
                {
                    if (m_draggedEntity != entity)
                    {
                        if (dragEventHandler->dragDropped != nullptr)
                        {
                            candidateDestinationEntities.push_back(entity);
                        }
                    }
                }
            }

            std::sort(
                candidateDestinationEntities.begin(), 
                candidateDestinationEntities.end(),
                [&](const entity_t& a, const entity_t& b)->bool
                {
                    TransformComponent* aTransform = Engine::ECS::Registry::GetComponent<Engine::ECS::TransformComponent>(a);
                    TransformComponent* bTransform = Engine::ECS::Registry::GetComponent<Engine::ECS::TransformComponent>(b);
                    return aTransform->GetWorldDepth() > bTransform->GetWorldDepth();
                });
            
            bool doConsume = false;
            for (entity_t entity : candidateDestinationEntities)
            {
                DragEventHandlerComponent* dragEventHandler = Registry::GetComponent<DragEventHandlerComponent>(entity);
                doConsume = dragEventHandler->dragDropped(m_draggedEntity, entity);
                if (doConsume)
                {
                    break;
                }
            }

            if (!doConsume && (dragEventHandler->dragEnd != nullptr))
            {
                dragEventHandler->dragEnd(m_draggedEntity);
            }

            m_currentState = State::Idle;   
        }
    }
    else
    {
        // This shouldn't happen
    }
}
}
}