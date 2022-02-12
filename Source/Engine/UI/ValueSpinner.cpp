#include "Engine/UI/ValueSpinner.hpp"

#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/GameObject.hpp"
#include "Engine/UI/Button.hpp"
#include "Engine/UI/Text.hpp"
#include "Engine/Vector.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>

namespace Engine
{
/**
 * @brief Constructor
 */
ValueSpinner::ValueSpinner()
    : m_gameObject(nullptr)
    , m_decreaseValueButton(nullptr)
    , m_increaseValueButton(nullptr)
    , m_valueText(nullptr)
    , m_currentValueIndex(-1)
    , m_values()
{
}

/**
 * @brief Destructor
 */
ValueSpinner::~ValueSpinner()
{
}

/**
 * @brief Initializes this spinner UI
 */
void ValueSpinner::Init()
{
    m_gameObject = new Engine::GameObject();

    ECS::TransformComponent *transform = m_gameObject->AddComponent<Engine::ECS::TransformComponent>();

    m_decreaseValueButton = new Engine::Button();
    m_decreaseValueButton->Init();
    m_decreaseValueButton->GetTransform()->parent = transform;
    m_decreaseValueButton->GetTransform()->localDepth = 1;
    m_decreaseValueButton->onPressed = std::bind(&ValueSpinner::OnDecreaseButtonPressed, this);
    m_decreaseValueButton->SetSize(20, 20);
    m_decreaseValueButton->SetText("-");
    m_decreaseValueButton->SetPosition(0.0f, 5.0f);

    m_valueText = new Engine::Text();
    m_valueText->Init();
    m_valueText->GetTransform()->parent = transform;
    m_valueText->GetTransform()->localDepth = 1;
    m_valueText->SetTextColor({1.0f, 1.0f, 1.0f, 1.0f});
    m_valueText->SetPosition({20.0f, 0.0f});

    m_increaseValueButton = new Engine::Button();
    m_increaseValueButton->Init();
    m_increaseValueButton->GetTransform()->parent = transform;
    m_increaseValueButton->GetTransform()->localDepth = 1;
    m_increaseValueButton->onPressed = std::bind(&ValueSpinner::OnIncreaseButtonPressed, this);
    m_increaseValueButton->SetSize(20, 20);
    m_increaseValueButton->SetText("+");
    m_increaseValueButton->SetPosition(70.0f, 5.0f);
    
    Refresh();
}

/**
 * @brief Cleans up the resources used by this wrapper class
 */
void ValueSpinner::Cleanup()
{
    if (m_valueText != nullptr)
    {
        m_valueText->Cleanup();
        delete m_valueText;
        m_valueText = nullptr;
    }

    if (m_increaseValueButton != nullptr)
    {
        m_increaseValueButton->Cleanup();
        delete m_increaseValueButton;
        m_increaseValueButton = nullptr;
    }

    if (m_decreaseValueButton != nullptr)
    {
        m_decreaseValueButton->Cleanup();
        delete m_decreaseValueButton;
        m_decreaseValueButton = nullptr;
    }

    if (m_gameObject != nullptr)
    {
        m_gameObject->Destroy();
        delete m_gameObject;
        m_gameObject = nullptr;
    }
}

/**
 * @brief Gets the currently selected value
 * @return Currently selected value
 */
std::string ValueSpinner::GetValue()
{
    int32_t numValues = m_values.size();
    if ((0 <= m_currentValueIndex) && (m_currentValueIndex < numValues))
    {
        return m_values[m_currentValueIndex];
    }
    return "";
}

/**
 * @brief Gets the index of the currently selected value
 * @return Index of the currently selected value
 */
int32_t ValueSpinner::GetValueIndex()
{
    return m_currentValueIndex;
}

/**
 * @brief Sets the index of the currently selected value
 * @param[in] newValueIndex New index
 */
void ValueSpinner::SetValueIndex(const int32_t &newValueIndex)
{
    m_currentValueIndex = newValueIndex;
    Refresh();
}

/**
 * @brief Sets the possible values for the spinner
 * @param[in] newChoices New set of possible values
 */
void ValueSpinner::SetValues(const std::vector<std::string> &newValues)
{
    m_values.clear();
    m_values.insert(m_values.begin(), newValues.begin(), newValues.end());
    m_currentValueIndex = -1;

    Refresh();
}

/**
 * @brief Refreshes the display of this UI
 */
void ValueSpinner::Refresh()
{
    if (m_valueText != nullptr)
    {
        int32_t numChoices = m_values.size();
        if ((0 <= m_currentValueIndex) && (m_currentValueIndex < numChoices))
        {
            m_valueText->SetText(m_values[m_currentValueIndex]);
        }
        else
        {
            m_valueText->SetText("");
        }
    }
}

/**
 * @brief Sets the local position of this UI relative to its parent
 * @param[in] newLocalPosition New local position
 */
void ValueSpinner::SetLocalPosition(const Engine::Vector2f& newLocalPosition)
{
    ECS::TransformComponent *transform = m_gameObject->GetComponent<ECS::TransformComponent>();
    if (transform != nullptr)
    {
        transform->localPosition = newLocalPosition;
    }
}

/**
 * @brief Sets the local position of this UI relative to its parent
 * @param[in] newLocalX New x-position
 * @param[in] newLocalY New y-position
 */
void ValueSpinner::SetLocalPosition(const float& newLocalX, const float& newLocalY)
{
    ECS::TransformComponent *transform = m_gameObject->GetComponent<ECS::TransformComponent>();
    if (transform != nullptr)
    {
        transform->localPosition.x() = newLocalX;
        transform->localPosition.y() = newLocalY;
    }
}

/**
 * @brief Sets the visibility of this UI
 * @param[in] isVisible Visibility flag
 */
void ValueSpinner::SetVisible(bool isVisible)
{
    if (m_decreaseValueButton != nullptr)
    {
        m_decreaseValueButton->SetVisible(isVisible);
    }
    if (m_increaseValueButton != nullptr)
    {
        m_increaseValueButton->SetVisible(isVisible);
    }
    if (m_valueText != nullptr)
    {
        m_valueText->SetVisible(isVisible);
    }
}

/**
 * @brief Gets the transform component of this UI
 * @return Transform component
 */
ECS::TransformComponent* ValueSpinner::GetTransform()
{
    return m_gameObject->GetComponent<ECS::TransformComponent>();
}

/**
 * @brief Handler function for when the decrease button was pressed
 */
void ValueSpinner::OnDecreaseButtonPressed()
{
    m_currentValueIndex = std::max(m_currentValueIndex - 1, 0);
    Refresh();
}

/**
 * @brief Handler function for when the increase button was pressed
 */
void ValueSpinner::OnIncreaseButtonPressed()
{
    int32_t maxIndex = m_values.size() - 1;
    m_currentValueIndex = std::min(m_currentValueIndex + 1, maxIndex);
    Refresh();
}
}
