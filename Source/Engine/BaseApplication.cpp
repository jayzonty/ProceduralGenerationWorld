#include "Engine/BaseApplication.hpp"

#include "Engine/Input.hpp"
#include "Engine/Matrix.hpp"
#include "Engine/MatrixTransform.hpp"
#include "Engine/WindowManager.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

namespace Engine
{
/**
 * @brief Constructor
 */
BaseApplication::BaseApplication()
    : m_isRunning(false)
    , m_activeScene(nullptr)
{
}

/**
 * @brief Destructor
 */
BaseApplication::~BaseApplication()
{
}

/**
 * @brief Runs the application.
 * @param[in] initialScene Initial scene
 */
void BaseApplication::Run(BaseScene* initialScene)
{
    // If the application is somehow running when this
    // function is called, don't allow to "re-run".
    if (m_isRunning)
    {
        return;
    }
    m_isRunning = true;

    if (initialScene == nullptr)
    {
        std::cout << "[Application] Initial scene is null! Exiting." << std::endl;
    }
    m_activeScene = initialScene;

    if (!Init())
    {
        std::cout << "[Application] Failed to initialize application!" << std::endl;
        return;
    }
    OnInit();

    m_activeScene->Init();

    double prevTime = glfwGetTime();

    Window* mainWindow = WindowManager::GetMainWindow();

    // Game loop
    while (!mainWindow->IsClosed())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - prevTime);
        prevTime = currentTime;

        m_activeScene->Update(deltaTime);
        m_activeScene->FixedUpdate(0.0f);

        m_activeScene->Draw();

        mainWindow->SwapBuffers();

        Input::Prepare();
        mainWindow->PollEvents();
    }

    OnCleanup();
    Cleanup();
}

/**
 * @brief Initializes the application.
 * @return Returns true if the initialization was successful.
 * Returns false otherwise.
 */
bool BaseApplication::Init()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cout << "[Application] Failed to initialize GLFW!" << std::endl;
        return false;
    }

    Window* mainWindow = WindowManager::GetMainWindow();
    if (!mainWindow->Init(800, 600, "Application"))
    {
        std::cout << "[Application] Failed to create GLFW window!" << std::endl;
        return false;
    }

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // --- Register callbacks ---
    GLFWwindow* windowHandle = mainWindow->GetWindowHandle();

    // Set the callback function for when the framebuffer size changed
	//glfwSetFramebufferSizeCallback(windowHandle, Application::FramebufferSizeChangedCallback);

	// Set the callback function for when a key was pressed
	glfwSetKeyCallback(windowHandle, Input::KeyCallback);

	// Register callback function for when a mouse button was pressed
	glfwSetMouseButtonCallback(windowHandle, Input::MouseButtonCallback);

    // Register callback function for when the mouse scroll wheel was scrolled
    glfwSetScrollCallback(windowHandle, Input::MouseScrollCallback);

	// Register callback function for when the mouse cursor's position changed
	glfwSetCursorPosCallback(windowHandle, Input::CursorCallback);

	// Register callback function for when the mouse cursor entered/left the window
	glfwSetCursorEnterCallback(windowHandle, Input::CursorEnterCallback);

    return true;
}

/**
 * @brief Cleans up the application.
 */
void BaseApplication::Cleanup()
{
    Window* mainWindow = WindowManager::GetMainWindow();
    mainWindow->Cleanup();
    glfwTerminate();
}
}
