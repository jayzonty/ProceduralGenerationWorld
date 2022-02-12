#pragma once

#include "Engine/ECS/Components/TextComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/GameObject.hpp"
#include "Engine/Graphics/Color.hpp"
#include "Engine/Vector.hpp"

#include <string>

namespace Engine
{
/**
 * Text UI
 */
class Text
{
private:
    /**
     * Game object for this text UI
     */
    Engine::GameObject *m_gameObject;

public:
    /**
     * @brief Constructor
     */
    Text();

    /**
     * @brief Destructor
     */
    ~Text();

    /**
     * @brief Initializes the text UI
     */
    void Init();

    /**
     * @brief Cleans up the resources used by this text UI
     */
    void Cleanup();

    /**
     * @brief Gets the transform component of this text
     * @return Transform component of this text
     */
    Engine::ECS::TransformComponent* GetTransform();

    /**
     * @brief Sets the text to be displayed
     * @param[in] newText New text to be displayed
     */
    void SetText(const std::string& newText);

    /**
     * @brief Sets the position of this text UI
     * @param[in] newPosition New position
     */
    void SetPosition(const Engine::Vector2f& newPosition);

    /**
     * @brief Sets the color of the text
     * @param[in] newColor New text color
     */
    void SetTextColor(const Engine::Color& newColor);

    /**
     * @brief Sets the visibility of this text UI
     * @param[in] isVisible Visibility of this text UI
     */
    void SetVisible(bool isVisible);
};
}
