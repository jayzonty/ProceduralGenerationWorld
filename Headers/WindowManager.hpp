#pragma once

#include "Window.hpp"

/**
 * Window manager class
 */
class WindowManager
{
private:
    /**
     * @brief Constructor
     */
    WindowManager();

public:
    // Delete copy constructor and copy operator
    WindowManager(const WindowManager&) = delete;
    void operator=(const WindowManager&) = delete;

    /**
     * @brief Gets the reference to the main window.
     * @return Reference to the main window
     */
    static Window* GetMainWindow();
};
