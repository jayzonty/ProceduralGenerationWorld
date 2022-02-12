#include "Engine/UI/Button.hpp"
#include "Engine/ECS/Components/MouseButtonEventHandlerComponent.hpp"
#include "Engine/ECS/Components/ShapeComponent.hpp"
#include "Engine/ECS/Components/TextComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/GameObject.hpp"
#include <functional>

namespace Engine
{
/**
 * @brief Constructor
 */
Button::Button()
    : m_gameObject(nullptr)
    , onPressed()
{
}

/**
 * @brief Destructor
 */
Button::~Button()
{
}

/**
 * @brief Initializes this Button object.
 */
void Button::Init()
{
    m_gameObject = new GameObject();

    m_gameObject->AddComponent<ECS::TransformComponent>();

    ECS::ShapeComponent *background = m_gameObject->AddComponent<ECS::ShapeComponent>();
    background->isVisible = true;
    background->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    background->type = ECS::ShapeComponent::Type::Rectangle;

    ECS::TextComponent *textComponent = m_gameObject->AddComponent<ECS::TextComponent>();
    textComponent->isVisible = true;
    textComponent->textColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    textComponent->origin = { 0.5f, 0.5f };

    ECS::MouseButtonEventHandlerComponent *mouseButtonEventHandlerComponent = m_gameObject->AddComponent<ECS::MouseButtonEventHandlerComponent>();
    mouseButtonEventHandlerComponent->isEnabled = true;
    mouseButtonEventHandlerComponent->onPressed = std::bind(&Button::MouseButtonPressedHandler, this, std::placeholders::_1, std::placeholders::_2);
}

/**
 * @brief Cleans up the resources used by this TileInfoPanel object.
 */
void Button::Cleanup()
{
    if (m_gameObject != nullptr)
    {
        m_gameObject->Destroy();
        delete m_gameObject;
        m_gameObject = nullptr;
    }
}

/**
 * @brief Gets the transform component of this button
 * @return Transform component of this button
 */
Engine::ECS::TransformComponent* Button::GetTransform()
{
    return m_gameObject->GetComponent<ECS::TransformComponent>();
}

/**
 * @brief Sets the visibility of this button.
 * @param[in] isVisible Flag indicating whether this button is visible or not
 */
void Button::SetVisible(bool isVisible)
{
    ECS::ShapeComponent *background = m_gameObject->GetComponent<ECS::ShapeComponent>();
    if (background != nullptr)
    {
        background->isVisible = isVisible;
    }

    ECS::TextComponent *textComponent = m_gameObject->GetComponent<ECS::TextComponent>();
    if (textComponent != nullptr)
    {
        textComponent->isVisible = isVisible;
    }
}

/**
 * @brief Sets the position of this button.
 * @param[in] x X-position
 * @param[in] y Y-position
 */
void Button::SetPosition(const float& x, const float& y)
{
    ECS::TransformComponent *transform = m_gameObject->GetComponent<ECS::TransformComponent>();
    if (transform != nullptr)
    {
        transform->localPosition.x() = x;
        transform->localPosition.y() = y;
    }
}

/**
 * @brief Sets the size of this button.
 * @param[in] width Button width
 * @param[in] height Button height
 */
void Button::SetSize(const float& width, const float& height)
{
    ECS::ShapeComponent *background = m_gameObject->GetComponent<ECS::ShapeComponent>();
    if (background != nullptr)
    {
        background->halfExtents.x() = width / 2.0f;
        background->halfExtents.y() = height / 2.0f;
    }

    ECS::MouseButtonEventHandlerComponent *mouseButtonEventHandlerComponent = m_gameObject->GetComponent<ECS::MouseButtonEventHandlerComponent>();
    if (mouseButtonEventHandlerComponent != nullptr)
    {
        mouseButtonEventHandlerComponent->bounds.halfExtents.x() = width / 2.0f;
        mouseButtonEventHandlerComponent->bounds.halfExtents.y() = height / 2.0f;
    }
}

/**
 * @brief Sets the text being displayed by this button.
 * @param[in] text Text
 */
void Button::SetText(const std::string& text)
{
    ECS::TextComponent *textComponent = m_gameObject->GetComponent<ECS::TextComponent>();
    if (textComponent != nullptr)
    {
        textComponent->text = text;
    }
}

bool Button::MouseButtonPressedHandler(const ECS::entity_t& entity, const Input::Button&)
{
    if (onPressed != nullptr)
    {
        onPressed();
    }

    return true;
}
}
