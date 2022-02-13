#include "Application.hpp"

#include "BaseApplication.hpp"
#include "Window.hpp"
#include "WindowManager.hpp"

/**
 * @brief Constructor
 */
Application::Application()
    : BaseApplication()
{
}

/**
 * @brief Destructor
 */
Application::~Application()
{
}

/**
 * @brief Overridable function called right after the base application initialization
 */
void Application::OnInit()
{
    Window *mainWindow = WindowManager::GetMainWindow();
    mainWindow->SetTitle("Voxel World");
}

/**
 * @brief Overridable function called right before the base appliction will be cleaned up
 */
void Application::OnCleanup()
{
}
