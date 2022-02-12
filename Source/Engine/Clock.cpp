#include "Engine/Clock.hpp"

#include <GLFW/glfw3.h>

namespace Engine
{
/**
 * @brief Constructor
 * @param[in] autoStart Flag indicating whether to start the timer
 * automatically or not
 */
Clock::Clock(bool autoStart)
    : m_startTime(0.0)
{
    if (autoStart)
    {
        Start();
    }
}

/**
 * @brief Destructor
 */
Clock::~Clock()
{
}

/**
 * @brief Starts the timer.
 */
void Clock::Start()
{
    m_startTime = glfwGetTime();
}

/**
 * @brief Resets the timer.
 * @return Time elapsed since this timer was started
 */
double Clock::Restart()
{
    double currentTime = glfwGetTime();
    double ret = currentTime - m_startTime;
    m_startTime = currentTime;
    return ret;
}

/**
 * @brief Gets the time elapsed since this timer was started.
 * @return Time elapsed since this timer was started
 */
double Clock::GetTime() const
{
    double currentTime = glfwGetTime();
    return currentTime - m_startTime;
}
}
