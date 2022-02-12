#pragma once

namespace Engine
{
/**
 * Class for a clock that counts up
 */
class Clock
{
private:
    /**
     * Time when this clock was last started
     */
    double m_startTime;

public:
    /**
     * @brief Constructor
     * @param[in] autoStart Flag indicating whether to start the timer
     * automatically or not
     */
    Clock(bool autoStart = false);

    /**
     * @brief Destructor
     */
    ~Clock();

    /**
     * @brief Starts the clock.
     */
    void Start();

    /**
     * @brief Resets the clock.
     * @return Time elapsed (in seconds) since this timer was started
     */
    double Restart();

    /**
     * @brief Gets the time elapsed (in seconds) since this clock was started.
     * @return Time elapsed since this clock was started
     */
    double GetTime() const;
};
}
