#pragma once

#include <functional>
#include <map>

namespace Engine
{
typedef int32_t EventListenerHandle;

/**
 * Event class
 */
template <class T>
class Event
{
    /**
     * Mapping of a handle and its corresponding listener
     */
    std::map<EventListenerHandle, std::function<T()>> m_listeners;

public:
    /**
     * @brief Constructor
     */
    Event()
        : m_listeners()
    {
    }

    /**
     * @brief Destructor
     */
    ~Event()
    {
        m_listeners.clear();
    }

    /**
     * @brief Adds a listener to the event
     * @param[in] listener Listener to add
     * @return Handle to the listener
     * @note This function currently does not account for "duplicate" listeners
     */
    EventListenerHandle AddListener(std::function<T()> listener)
    {
        EventListenerHandle handle = m_listeners.size();
        m_listeners[handle] = listener;
        return handle;
    }

    /**
     * @brief Removes the listener identified by the specified handle
     * @param[in] handle Handle to the listener to remove
     */
    void RemoveListener(const EventListenerHandle& handle)
    {
        m_listeners.erase(handle);
    }

    /**
     * @brief Operator overload function for invoking all the listeners
     * assigned to this event
     */
    T operator()() const
    {
        for (auto& it : m_listeners)
        {
            it.second();
        }
    }
};
}
