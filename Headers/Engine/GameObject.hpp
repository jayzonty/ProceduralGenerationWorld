#pragma once

#include "Engine/ECS/ECS.hpp"

#include <iostream>

namespace Engine
{
/**
 * Game object
 */
class GameObject
{
private:
    /**
     * Entity handle
     */
    ECS::entity_t m_entity;

    /**
     * Flag indicating whether this game object has been destroyed or not
     */
    bool m_isDestroyed;

public:
    /**
     * @brief Constructor
     */
    GameObject()
        : m_entity(0)
        , m_isDestroyed(false)
    {
        m_entity = ECS::Registry::CreateEntity();
    }

    /**
     * @brief Copy constructor
     * @param[in] other Game object to copy from
     */
    GameObject(const GameObject& other)
    {
        m_entity = other.m_entity;
    }

    /**
     * @brief Template function to add the specified component to this game object.
     * @return Component data that was just created
     */
    template <typename T>
    T* AddComponent()
    {
        if (m_isDestroyed)
        {
            std::cerr << "[GameObject] Trying to add a component to a destroyed game object!" << std::endl;
            return nullptr;
        }

        return ECS::Registry::AddComponent<T>(m_entity);
    }

    /**
     * @brief Template function to get the specified component from this game object.
     * @return Component data of the specified type. Returns nullptr if the game object does not have the specified component.
     */
    template <typename T>
    T* GetComponent()
    {
        if (m_isDestroyed)
        {
            std::cerr << "[GameObject] Trying to get a component from a destroyed game object!" << std::endl;
            return nullptr;
        }

        return ECS::Registry::GetComponent<T>(m_entity);
    }

    /**
     * @brief Template function to remove the specified component from the game object.
     */
    template <typename T>
    void RemoveComponent()
    {
        if (m_isDestroyed)
        {
            std::cerr << "[GameObject] Trying to remove a component from a destroyed game object!" << std::endl;
            return;
        }

        ECS::Registry::RemoveComponent<T>(m_entity);
    }

    /**
     * @brief Destroys this game object.
     */
    void Destroy()
    {
        if (m_isDestroyed)
        {
            return;
        }

        ECS::Registry::DestroyEntity(m_entity);
        m_isDestroyed = true;
    }
};
}
