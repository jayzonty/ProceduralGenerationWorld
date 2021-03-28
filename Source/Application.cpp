#include "Application.hpp"

#include "Input.hpp"
#include "MainScene.hpp"

#include <iostream>

/// <summary>
/// Constructor
/// </summary>
Application::Application()
	: m_window(nullptr)
	, m_currentScene(nullptr)
{
}

/// <summary>
/// Destructor
/// </summary>
Application::~Application()
{
	if (m_currentScene != nullptr)
	{
		// TODO: Have some sort of scene manager for managing scene resources
		delete m_currentScene;
	}
}

/// <summary>
/// Run application
/// </summary>
void Application::Run()
{
	Initialize();

	// TODO: Have a scene manager that manages creating scenes
	m_currentScene = new MainScene();

	// Start scene
	m_currentScene->Start();

	// Take note of the current time before
	// running the render loop
	double prevTime = glfwGetTime();

	// Render loop
	while (!glfwWindowShouldClose(m_window))
	{
		// If escape is pressed, close the window
		if (Input::IsPressed(Input::Key::ESCAPE))
		{
			glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		}

		// For now, just use a basic delta time calculation
		double deltaTimeAsDouble = glfwGetTime() - prevTime;
		prevTime = glfwGetTime();
		float deltaTime = static_cast<float>(deltaTimeAsDouble);

		// Update current scene
		m_currentScene->Update(deltaTime);

		// Draw current scene
		m_currentScene->Draw();

		// Tell GLFW to swap the screen buffer with the offscreen buffer
		glfwSwapBuffers(m_window);

		// Update input manager state before polling the new state
		Input::Prepare();

		// Poll for events
		glfwPollEvents();
	}

	// Finish scene
	m_currentScene->Finish();

	Finish();
}

/// <summary>
/// Initialize application
/// </summary>
bool Application::Initialize()
{
	// Initialize GLFW
	int glfwInitStatus = glfwInit();
	if (glfwInitStatus == GLFW_FALSE)
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return false;
	}

	// Tell GLFW that we prefer to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW that we prefer to use the modern OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tell GLFW to create a window
	int windowWidth = 800;
	int windowHeight = 600;
	m_window = glfwCreateWindow(windowWidth, windowHeight, "Hello Triangle", nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return false;
	}

	// Tell GLFW to use the OpenGL context that was assigned to the window that we just created
	glfwMakeContextCurrent(m_window);

	// Set the callback function for when the framebuffer size changed
	glfwSetFramebufferSizeCallback(m_window, Application::FramebufferSizeChangedCallback);

	// Initialize input manager
	Input::Initialize();

	// Set the callback function for when a key was pressed
	glfwSetKeyCallback(m_window, Input::KeyCallback);

	// Register callback function for when a mouse button was pressed
	glfwSetMouseButtonCallback(m_window, Input::MouseButtonCallback);

	// Register callback function for when the mouse cursor's position changed
	glfwSetCursorPosCallback(m_window, Input::CursorCallback);

	// Register callback function for when the mouse cursor entered/left the window
	glfwSetCursorEnterCallback(m_window, Input::CursorEnterCallback);

	// Disable the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Tell GLAD to load the OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	// Tell OpenGL the dimensions of the region where stuff will be drawn.
	// For now, tell OpenGL to use the whole screen
	glViewport(0, 0, windowWidth, windowHeight);

	return true;
}

/// <summary>
/// Finish application
/// </summary>
void Application::Finish()
{
	// Re-enable the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Cleanup resources used by the input manager
	Input::Cleanup();

	// Tell GLFW to clean itself up before exiting the application
	glfwTerminate();
}

/// <summary>
/// Function for handling the event when the size of the framebuffer changed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="width">New width</param>
/// <param name="height">New height</param>
void Application::FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	// Whenever the size of the framebuffer changed (due to window resizing, etc.),
	// update the dimensions of the region to the new size
	glViewport(0, 0, width, height);
}
