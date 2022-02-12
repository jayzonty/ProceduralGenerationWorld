#pragma once

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
 * Class for a button UI
 */
class Button
{
private:
    /**
     * Game object
     */
    GameObject *m_gameObject;

public:
    std::function<void()> onPressed;

public:
    /**
     * @brief Constructor
     */
    Button();

    /**
     * @brief Destructor
     */
    ~Button();

    /**
     * @brief Initializes this Button object.
     */
    void Init();

    /**
     * @brief Cleans up the resources used by this TileInfoPanel object.
     */
    void Cleanup();

    /**
     * @brief Gets the transform component of this button
     * @return Transform component of this button
     */
    Engine::ECS::TransformComponent* GetTransform();

    /**
     * @brief Sets the visibility of this button.
     * @param[in] isVisible Flag indicating whether this button is visible or not
     */
    void SetVisible(bool isVisible);

    /**
     * @brief Sets the position of this button.
     * @param[in] x X-position
     * @param[in] y Y-position
     */
    void SetPosition(const float& x, const float& y);

    /**
     * @brief Sets the size of this button.
     * @param[in] width Button width
     * @param[in] height Button height
     */
    void SetSize(const float& width, const float& height);

    /**
     * @brief Sets the text being displayed by this button.
     * @param[in] text Text
     */
    void SetText(const std::string& text);

private:
    bool MouseButtonPressedHandler(const ECS::entity_t& entity, const Input::Button&);
};
}
