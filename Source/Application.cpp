#include "Application.hpp"

#include "BaseApplication.hpp"
#include "Constants.hpp"
//#include "Scenes/MainScene.hpp"
#include "Scenes/SandboxScene.hpp"
#include "Window.hpp"
#include "WindowManager.hpp"

/**
 * @brief Constructor
 */
Application::Application()
    : BaseApplication()
    //, m_mainScene(nullptr)
    , m_sandboxScene(nullptr)
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

    //m_mainScene = new MainScene(GetSceneManager());
    m_sandboxScene = new SandboxScene(GetSceneManager());

    //GetSceneManager()->RegisterScene(Constants::MAIN_SCENE_ID, m_mainScene);
    GetSceneManager()->RegisterScene(Constants::SANDBOX_SCENE_ID, m_sandboxScene);

    GetSceneManager()->SwitchToScene(Constants::SANDBOX_SCENE_ID);
}

/**
 * @brief Overridable function called right before the base appliction will be cleaned up
 */
void Application::OnCleanup()
{
    //delete m_mainScene;
    //m_mainScene = nullptr;

    delete m_sandboxScene;
    m_sandboxScene = nullptr;
}
