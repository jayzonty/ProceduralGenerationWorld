#include "Engine/UI/Spinner.hpp"

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
Spinner::Spinner()
    : m_gameObject(nullptr)
    , m_decreaseValueButton(nullptr)
    , m_increaseValueButton(nullptr)
    , m_valueText(nullptr)
    , m_min(0)
    , m_max(1)
    , m_value(0)
    , m_increment(0)
{
}

/**
 * @brief Destructor
 */
Spinner::~Spinner()
{
}

/**
 * @brief Initializes this spinner UI
 */
void Spinner::Init()
{
    m_gameObject = new Engine::GameObject();

    ECS::TransformComponent *transform = m_gameObject->AddComponent<Engine::ECS::TransformComponent>();

    m_decreaseValueButton = new Engine::Button();
    m_decreaseValueButton->Init();
    m_decreaseValueButton->GetTransform()->parent = transform;
    m_decreaseValueButton->GetTransform()->localDepth = 1;
    m_decreaseValueButton->onPressed = std::bind(&Spinner::OnDecreaseButtonPressed, this);
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
    m_increaseValueButton->onPressed = std::bind(&Spinner::OnIncreaseButtonPressed, this);
    m_increaseValueButton->SetSize(20, 20);
    m_increaseValueButton->SetText("+");
    m_increaseValueButton->SetPosition(70.0f, 5.0f);
    
    Refresh();
}

/**
 * @brief Cleans up the resources used by this wrapper class
 */
void Spinner::Cleanup()
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
 * @brief Gets the minimum value
 * @return Minimum value
 */
int32_t Spinner::GetMin()
{
    return m_min;
}

/**
 * @brief Sets the minimum value
 * @param[in] newMin New minimum value
 */
void Spinner::SetMin(const int32_t &newMin)
{
    if (newMin > m_max)
    {
        m_min = m_max;
        m_max = newMin;
    }
    else
    {
        m_min = newMin;
    }
}

/**
 * @brief Gets the maximum value
 * @return Maximum value
 */
int32_t Spinner::GetMax()
{
    return m_max;
}

/**
 * @brief Sets the maximum value
 * @param[in] newMax New maximum value
 */
void Spinner::SetMax(const int32_t &newMax)
{
    if (newMax < m_min)
    {
        m_max = m_min;
        m_min = newMax;
    }
    else
    {
        m_max = newMax;
    }
}

/**
 * @brief Gets the current value
 * @return Current value
 */
int32_t Spinner::GetValue()
{
    return m_value;
}

/**
 * @brief Sets the current value
 * @param[in] newValue New value
 */
void Spinner::SetValue(const int32_t &newValue)
{
    m_value = newValue;
    m_value = std::clamp(newValue, m_min, m_max);
}

/**
 * @brief Gets the increment value
 * @return Increment value
 */
int32_t Spinner::GetIncrement()
{
    return m_increment;
}

/**
 * @brief Sets the increment value
 * @param[in] newIncrement New increment value
 */
void Spinner::SetIncrement(const int32_t &newIncrement)
{
    m_increment = newIncrement;
}

/**
 * @brief Refreshes the display of this UI
 */
void Spinner::Refresh()
{
    if (m_valueText != nullptr)
    {
        m_valueText->SetText(std::to_string(m_value));
    }
}

/**
 * @brief Sets the local position of this UI relative to its parent
 * @param[in] newLocalPosition New local position
 */
void Spinner::SetLocalPosition(const Engine::Vector2f& newLocalPosition)
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
void Spinner::SetLocalPosition(const float& newLocalX, const float& newLocalY)
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
void Spinner::SetVisible(bool isVisible)
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
ECS::TransformComponent* Spinner::GetTransform()
{
    return m_gameObject->GetComponent<ECS::TransformComponent>();
}

/**
 * @brief Handler function for when the decrease button was pressed
 */
void Spinner::OnDecreaseButtonPressed()
{
    m_value = std::max(m_value - m_increment, m_min);
    Refresh();
}

/**
 * @brief Handler function for when the increase button was pressed
 */
void Spinner::OnIncreaseButtonPressed()
{
    m_value = std::min(m_value + m_increment, m_max);
    Refresh();
}
}
