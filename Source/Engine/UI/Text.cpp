#include "Engine/UI/Text.hpp"

#include "Engine/ECS/Components/TextComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/GameObject.hpp"

namespace Engine
{
/**
 * @brief Constructor
 */
Text::Text()
    : m_gameObject(nullptr)
{
}

/**
 * @brief Destructor
 */
Text::~Text()
{
}

/**
 * @brief Initializes the text UI
 */
void Text::Init()
{
    if (m_gameObject != nullptr)
    {
        return;
    }

    m_gameObject = new Engine::GameObject();

    m_gameObject->AddComponent<Engine::ECS::TransformComponent>();

    ECS::TextComponent *textComponent = m_gameObject->AddComponent<Engine::ECS::TextComponent>();
    textComponent->isVisible = true;
}

/**
 * @brief Cleans up the resources used by this text UI
 */
void Text::Cleanup()
{
    if (m_gameObject == nullptr)
    {
        return;
    }

    m_gameObject->Destroy();
    delete m_gameObject;
    m_gameObject = nullptr;
}

/**
 * @brief Gets the transform component of this text
 * @return Transform component of this text
 */
Engine::ECS::TransformComponent* Text::GetTransform()
{
    return m_gameObject->GetComponent<ECS::TransformComponent>();
}

/**
 * @brief Sets the text to be displayed
 * @param[in] newText New text to be displayed
 */
void Text::SetText(const std::string &newText)
{
    ECS::TextComponent *textComponent = m_gameObject->GetComponent<ECS::TextComponent>();
    if (textComponent != nullptr)
    {
        textComponent->text = newText;
    }
}

/**
 * @brief Sets the position of this text UI
 * @param[in] newPosition New position
 */
void Text::SetPosition(const Engine::Vector2f &newPosition)
{
    ECS::TransformComponent *transform = m_gameObject->GetComponent<ECS::TransformComponent>();
    if (transform != nullptr)
    {
        transform->localPosition = newPosition;
    }
}

/**
 * @brief Sets the color of the text
 * @param[in] newColor New text color
 */
void Text::SetTextColor(const Engine::Color &newColor)
{
    ECS::TextComponent *textComponent = m_gameObject->GetComponent<ECS::TextComponent>();
    if (textComponent != nullptr)
    {
        textComponent->textColor = newColor;
    }
}

/**
 * @brief Sets the visibility of this text UI
 * @param[in] isVisible Visibility of this text UI
 */
void Text::SetVisible(bool isVisible)
{
    ECS::TextComponent *textComponent = m_gameObject->GetComponent<ECS::TextComponent>();
    if (textComponent != nullptr)
    {
        textComponent->isVisible = isVisible;
    }
}
}
