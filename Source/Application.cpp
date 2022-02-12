#include "Application.hpp"

#include "Engine/BaseApplication.hpp"
#include "Engine/Window.hpp"
#include "Engine/WindowManager.hpp"

/**
 * @brief Constructor
 */
Application::Application()
    : Engine::BaseApplication()
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
    Engine::Window *mainWindow = Engine::WindowManager::GetMainWindow();
    mainWindow->SetTitle("Voxel World");
}

/**
 * @brief Overridable function called right before the base appliction will be cleaned up
 */
void Application::OnCleanup()
{
}
