#include "BaseApplication.hpp"

#include "Input.hpp"
#include "SceneManager.hpp"
#include "WindowManager.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

/**
 * @brief Constructor
 */
BaseApplication::BaseApplication()
    : m_isRunning(false)
    , m_sceneManager()
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
 */
void BaseApplication::Run()
{
    // If the application is somehow running when this
    // function is called, don't allow to "re-run".
    if (m_isRunning)
    {
        return;
    }
    m_isRunning = true;

    if (!Init())
    {
        std::cout << "[Application] Failed to initialize application!" << std::endl;
        return;
    }
    OnInit();

    m_sceneManager.GetActiveScene()->Init();

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

        m_sceneManager.GetActiveScene()->Update(deltaTime);
        m_sceneManager.GetActiveScene()->FixedUpdate(0.0f);

        m_sceneManager.GetActiveScene()->Draw();

        mainWindow->SwapBuffers();

        Input::Prepare();
        mainWindow->PollEvents();
    }

    m_sceneManager.GetActiveScene()->Cleanup();
    OnCleanup();
    Cleanup();
}

/**
 * @brief Gets the scene manager for this application
 * @return Reference to the scene manager
 */
SceneManager* BaseApplication::GetSceneManager()
{
    return &m_sceneManager;
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

    // Disable the cursor.
    // TODO: Maybe have a function inside the window class for this
    glfwSetInputMode(mainWindow->GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
