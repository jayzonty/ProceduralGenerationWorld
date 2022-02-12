#pragma once

#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/GameObject.hpp"
#include "Engine/UI/Button.hpp"
#include "Engine/UI/Text.hpp"

#include <cstdint>

namespace Engine
{
/**
 * Wrapper class for a spinner UI for selecting from a
 * list of possible values
 */
class ValueSpinner
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
     * Index of the current value
     */
    int32_t m_currentValueIndex;

    /**
     * List of choices
     */
    std::vector<std::string> m_values;

public:
    /**
     * @brief Constructor
     */
    ValueSpinner();

    /**
     * @brief Destructor
     */
    ~ValueSpinner();

    /**
     * @brief Initializes this spinner UI
     */
    void Init();

    /**
     * @brief Cleans up the resources used by this wrapper class
     */
    void Cleanup();

    /**
     * @brief Gets the currently selected value
     * @return Currently selected value
     */
    std::string GetValue();

    /**
     * @brief Gets the index of the currently selected value
     * @return Index of the currently selected value
     */
    int32_t GetValueIndex();

    /**
     * @brief Sets the index of the currently selected value
     * @param[in] newValueIndex New index
     */
    void SetValueIndex(const int32_t& newValueIndex);

    /**
     * @brief Sets the possible values for the spinner
     * @param[in] newChoices New set of possible values
     */
    void SetValues(const std::vector<std::string>& newValues);

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
