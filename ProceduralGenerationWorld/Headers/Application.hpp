#pragma once

#include "SceneBase.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/// <summary>
/// Application class
/// </summary>
class Application
{
private:
	/// <summary>
	/// Window
	/// </summary>
	GLFWwindow* m_window;

	/// <summary>
	/// Current scene
	/// </summary>
	SceneBase* m_currentScene;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	Application();

	/// <summary>
	/// Destructor
	/// </summary>
	~Application();

	/// <summary>
	/// Run application
	/// </summary>
	void Run();

private:
	/// <summary>
	/// Initialize application
	/// </summary>
	/// <returns>Returns true if initialization was successful. False otherwise.</returns>
	bool Initialize();

	/// <summary>
	/// Finish application
	/// </summary>
	void Finish();

	/// <summary>
	/// Function for handling the event when the size of the framebuffer changed.
	/// </summary>
	/// <param name="window">Reference to the window</param>
	/// <param name="width">New width</param>
	/// <param name="height">New height</param>
	static void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height);
};
