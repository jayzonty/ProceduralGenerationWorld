#pragma once

#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/GameObject.hpp"
#include "Engine/UI/Button.hpp"
#include "Engine/UI/Text.hpp"

#include <cstdint>

namespace Engine
{
/**
 * Wrapper class for a standard spinner UI
 */
class Spinner
{
private:
    /**
     * Spinner game object
     */
    Engine::GameObject *m_gameObject;

    /**
     * Button for decreasing the value
     */
    Engine::Button *m_decreaseValueButton;

    /**
     * Button for increasing the value
     */
    Engine::Button *m_increaseValueButton;

    /**
     * Text for displaying the current value
     */
    Engine::Text *m_valueText;

    /**
     * Minimum value
     */
    int32_t m_min;

    /**
     * Maximum value
     */
    int32_t m_max;

    /**
     * Current value
     */
    int32_t m_value;

    /**
     * Increment value
     */
    int32_t m_increment;

public:
    /**
     * @brief Constructor
     */
    Spinner();

    /**
     * @brief Destructor
     */
    ~Spinner();

    /**
     * @brief Initializes this spinner UI
     */
    void Init();

    /**
     * @brief Cleans up the resources used by this wrapper class
     */
    void Cleanup();

    /**
     * @brief Gets the minimum value
     * @return Minimum value
     */
    int32_t GetMin();

    /**
     * @brief Sets the minimum value
     * @param[in] newMin New minimum value
     */
    void SetMin(const int32_t& newMin);

    /**
     * @brief Gets the maximum value
     * @return Maximum value
     */
    int32_t GetMax();

    /**
     * @brief Sets the maximum value
     * @param[in] newMax New maximum value
     */
    void SetMax(const int32_t& newMax);

    /**
     * @brief Gets the current value
     * @return Current value
     */
    int32_t GetValue();

    /**
     * @brief Sets the current value
     * @param[in] newValue New value
     */
    void SetValue(const int32_t& newValue);

    /**
     * @brief Gets the increment value
     * @return Increment value
     */
    int32_t GetIncrement();

    /**
     * @brief Sets the increment value
     * @param[in] newIncrement New increment value
     */
    void SetIncrement(const int32_t& newIncrement);

    /**
     * @brief Refreshes the display of this UI
     */
    void Refresh();

    /**
     * @brief Sets the local position of this UI relative to its parent
     * @param[in] newLocalPosition New local position
     */
    void SetLocalPosition(const Engine::Vector2f& newLocalPosition);

    /**
     * @brief Sets the local position of this UI relative to its parent
     * @param[in] newLocalX New x-position
     * @param[in] newLocalY New y-position
     */
    void SetLocalPosition(const float& newLocalX, const float& newLocalY);

    /**
     * @brief Sets the visibility of this UI
     * @param[in] isVisible Visibility flag
     */
    void SetVisible(bool isVisible);

    /**
     * @brief Gets the transform component of this UI
     * @return Transform component
     */
    ECS::TransformComponent* GetTransform();

private:
    /**
     * @brief Handler function for when the decrease button was pressed
     */
    void OnDecreaseButtonPressed();

    /**
     * @brief Handler function for when the increase button was pressed
     */
    void OnIncreaseButtonPressed();
};
}
