#include "Input.hpp"

/// <summary>
/// Static instance for this singleton class
/// </summary>
Input* Input::m_instance = nullptr;

/// <summary>
/// Constructor
/// </summary>
Input::Input()
	: m_pressedKeys()
	, m_releasedKeys()
	, m_heldKeys()
	, m_mousePositionX(0)
	, m_mousePositionY(0)
{
}

/// <summary>
/// Destructor
/// </summary>
Input::~Input()
{
}

/// <summary>
/// Is the key/button just pressed during this frame?
/// </summary>
/// <param name="key">Key to query</param>
/// <returns>True if the key was just pressed during this frame. False otherwise.</returns>
bool Input::IsPressed(int key)
{
	if (nullptr == m_instance)
	{
		return false;
	}

	return (m_instance->m_pressedKeys.find(key) !=
		m_instance->m_pressedKeys.end());
}

/// <summary>
/// Is the key/button just released in this frame?
/// </summary>
/// <param name="key">Key to query</param>
/// <returns>True if the key was released this frame. False otherwise.</returns>
bool Input::IsReleased(int key)
{
	if (nullptr == m_instance)
	{
		return false;
	}

	return (m_instance->m_releasedKeys.find(key) !=
		m_instance->m_releasedKeys.end());
}

/// <summary>
/// Is the key/button being pressed/held down?
/// </summary>
/// <param name="key">Key to query</param>
/// <returns>True if the key is being held down. False otherwise.</returns>
bool Input::IsDown(int key)
{
	if (nullptr == m_instance)
	{
		return false;
	}

	return (m_instance->m_heldKeys.find(key) !=
		m_instance->m_heldKeys.end());
}

/// <summary>
/// Gets the mouse cursor's position in the current frame
/// </summary>
/// <param name="mouseX">Mouse cursor's x-position</param>
/// <param name="mouseY">Mouse cursor's y-position</param>
void Input::GetMousePosition(int* mouseX, int* mouseY)
{
	if (nullptr == m_instance)
	{
		return;
	}

	if (nullptr != mouseX)
	{
		*mouseX = m_instance->m_mousePositionX;
	}
	if (nullptr != mouseY)
	{
		*mouseY = m_instance->m_mousePositionY;
	}
}

/// <summary>
/// Gets the mouse cursor's x-position in the current frame
/// </summary>
/// <returns>Mouse cursor's x-position</returns>
int Input::GetMouseX()
{
	if (nullptr == m_instance)
	{
		return 0;
	}

	return m_instance->m_mousePositionX;
}

/// <summary>
/// Gets the mouse cursor's y-position in the current frame
/// </summary>
/// <returns>Mouse cursor's y-position</returns>
int Input::GetMouseY()
{
	if (nullptr == m_instance)
	{
		return 0;
	}

	return m_instance->m_mousePositionY;
}

/// <summary>
/// Initializes the input manager
/// </summary>
void Input::Initialize()
{
	if (nullptr == m_instance)
	{
		m_instance = new Input();
	}
}

/// <summary>
/// Cleans up the resources used by the input manager
/// </summary>
void Input::Cleanup()
{
	delete m_instance;
	m_instance = nullptr;
}

/// <summary>
/// Prepare the input manager before polling for the new state
/// </summary>
void Input::Prepare()
{
	if (nullptr == m_instance)
	{
		return;
	}

	m_instance->m_pressedKeys.clear();
	m_instance->m_releasedKeys.clear();
}

/// <summary>
/// GLFW callback function for when a key event happened
/// </summary>
/// <param name="window">Reference to the GLFW window that received the event</param>
/// <param name="key">Key</param>
/// <param name="scanCode">Scan code</param>
/// <param name="action">Action</param>
/// <param name="mods">Modifiers</param>
void Input::KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (nullptr == m_instance)
	{
		return;
	}

	if (action == GLFW_PRESS)
	{
		m_instance->m_pressedKeys.insert(key);
		m_instance->m_heldKeys.insert(key);
	}
	else if (action == GLFW_RELEASE)
	{
		m_instance->m_pressedKeys.erase(key);
		m_instance->m_heldKeys.erase(key);

		m_instance->m_releasedKeys.insert(key);
	}
}

/// <summary>
/// GLFW callback function for when a mouse button event happened
/// </summary>
/// <param name="window">Reference to the GLFW window that received the event</param>
/// <param name="button">Mouse button</param>
/// <param name="action">Action</param>
/// <param name="mods">Modifiers</param>
void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (nullptr == m_instance)
	{
		return;
	}

	if (action == GLFW_PRESS)
	{
		m_instance->m_pressedKeys.insert(button);
	}
	else if (action == GLFW_REPEAT)
	{
		m_instance->m_pressedKeys.erase(button);
		m_instance->m_heldKeys.insert(button);
	}
	else if (action == GLFW_RELEASE)
	{
		m_instance->m_pressedKeys.erase(button);
		m_instance->m_heldKeys.erase(button);
		m_instance->m_releasedKeys.insert(button);
	}
}

/// <summary>
/// GLFW callback function for when a mouse cursor event happened
/// </summary>
/// <param name="window">Reference to the GLFW window that received the event</param>
/// <param name="xPos">Mouse cursor x-position</param>
/// <param name="yPos">Mouse cursor y-position</param>
void Input::CursorCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (nullptr == m_instance)
	{
		return;
	}

	m_instance->m_mousePositionX = static_cast<int>(floor(xPos));
	m_instance->m_mousePositionY = static_cast<int>(floor(yPos));
}

/// <summary>
/// GLFW callback function for when the mouse cursor entered or left the window
/// </summary>
/// <param name="window">Reference to the GLFW window that received the event</param>
/// <param name="entered">Integer indicating whether the cursor entered (1) or left (0)</param>
void Input::CursorEnterCallback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		m_instance->m_mousePositionX = static_cast<int>(mouseX);
		m_instance->m_mousePositionY = static_cast<int>(mouseY);
	}
}
