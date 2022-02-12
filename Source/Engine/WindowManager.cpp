#include "Engine/WindowManager.hpp"

namespace Engine
{
/**
 * @brief Constructor
 */
WindowManager::WindowManager()
{
}

/**
 * @brief Gets the reference to the main window.
 * @return Reference to the main window
 */
Window* WindowManager::GetMainWindow()
{
    static Window window;
    return &window;
}
}
