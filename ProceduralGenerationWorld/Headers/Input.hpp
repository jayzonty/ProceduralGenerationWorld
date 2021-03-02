#pragma once

#include <GLFW/glfw3.h>

#include <unordered_set>

/// <summary>
/// Class for handling input
/// </summary>
class Input
{
public:
	/// <summary>
	/// Key enum
	/// </summary>
	enum Key
	{
		UP = GLFW_KEY_UP,
		DOWN = GLFW_KEY_DOWN,
		LEFT = GLFW_KEY_LEFT,
		RIGHT = GLFW_KEY_RIGHT,

		SPACE = GLFW_KEY_SPACE,

		A = GLFW_KEY_A,
		D = GLFW_KEY_D,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		W = GLFW_KEY_W,

		ESCAPE = GLFW_KEY_ESCAPE
	};

	/// <summary>
	/// Mouse button enum
	/// </summary>
	enum Button
	{
		LEFT_MOUSE = GLFW_MOUSE_BUTTON_LEFT,
		MIDDLE_MOUSE = GLFW_MOUSE_BUTTON_MIDDLE,
		RIGHT_MOUSE = GLFW_MOUSE_BUTTON_RIGHT
	};

private:
	/// <summary>
	/// Static instance for this singleton class
	/// </summary>
	static Input* m_instance;

	/// <summary>
	/// Set of keys that are pressed in the current frame
	/// </summary>
	std::unordered_set<int> m_pressedKeys;

	/// <summary>
	/// Set of keys that were released in the current frame
	/// </summary>
	std::unordered_set<int> m_releasedKeys;

	/// <summary>
	/// Set of keys that are held down in the current frame
	/// </summary>
	std::unordered_set<int> m_heldKeys;

	/// <summary>
	/// Mouse cursor's x-position
	/// </summary>
	int m_mousePositionX;

	/// <summary>
	/// Mouse cursor's y-position
	/// </summary>
	int m_mousePositionY;

	/// <summary>
	/// Mouse cursor's change in x-position
	/// </summary>
	int m_mouseDeltaX;

	/// <summary>
	/// Mouse cursor's change in y-position
	/// </summary>
	int m_mouseDeltaY;

	/// <summary>
	/// Constructor
	/// </summary>
	Input();

public:
	/// <summary>
	/// Destructor
	/// </summary>
	~Input();

	/// <summary>
	/// Is the key/button just pressed during this frame?
	/// </summary>
	/// <param name="key">Key to query</param>
	/// <returns>True if the key was just pressed during this frame. False otherwise.</returns>
	static bool IsPressed(int key);

	/// <summary>
	/// Is the key/button just released in this frame?
	/// </summary>
	/// <param name="key">Key to query</param>
	/// <returns>True if the key was released this frame. False otherwise.</returns>
	static bool IsReleased(int key);

	/// <summary>
	/// Is the key/button being pressed/held down?
	/// </summary>
	/// <param name="key">Key to query</param>
	/// <returns>True if the key is being held down. False otherwise.</returns>
	static bool IsDown(int key);

	/// <summary>
	/// Gets the mouse cursor's position in the current frame
	/// </summary>
	/// <param name="mouseX">Mouse cursor's x-position</param>
	/// <param name="mouseY">Mouse cursor's y-position</param>
	static void GetMousePosition(int* mouseX, int* mouseY);

	/// <summary>
	/// Gets the mouse cursor's x-position in the current frame
	/// </summary>
	/// <returns>Mouse cursor's x-position</returns>
	static int GetMouseX();

	/// <summary>
	/// Gets the mouse cursor's y-position in the current frame
	/// </summary>
	/// <returns>Mouse cursor's y-position</returns>
	static int GetMouseY();

	/// <summary>
	/// Gets the change in mouse cursor's position between the
	/// previous frame and the current frame
	/// </summary>
	/// <param name="mouseDeltaX">Pointer to the variable where the change in x-position will be stored</param>
	/// <param name="mouseDeltaY">Pointer to the variable where the change in y-position will be stored</param>
	static void GetMouseDelta(int* mouseDeltaX, int* mouseDeltaY);

	/// <summary>
	/// Gets the change in mouse cursor's x-position
	/// </summary>
	/// <returns>Amount of change in x-position</returns>
	static int GetMouseDeltaX();

	/// <summary>
	/// Gets the change in mouse cursor's y-position
	/// </summary>
	/// <returns>Amount of change in y-position</returns>
	static int GetMouseDeltaY();
	
	/// <summary>
	/// Initializes the input manager
	/// </summary>
	static void Initialize();

	/// <summary>
	/// Cleans up the resources used by the input manager
	/// </summary>
	static void Cleanup();

	/// <summary>
	/// Prepare the input manager before polling for the new state
	/// </summary>
	static void Prepare();

	/// <summary>
	/// GLFW callback function for when a key event happened
	/// </summary>
	/// <param name="window">Reference to the GLFW window that received the event</param>
	/// <param name="key">Key</param>
	/// <param name="scanCode">Scan code</param>
	/// <param name="action">Action</param>
	/// <param name="mods">Modifiers</param>
	static void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

	/// <summary>
	/// GLFW callback function for when a mouse button event happened
	/// </summary>
	/// <param name="window">Reference to the GLFW window that received the event</param>
	/// <param name="button">Mouse button</param>
	/// <param name="action">Action</param>
	/// <param name="mods">Modifiers</param>
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	/// <summary>
	/// GLFW callback function for when a mouse cursor event happened
	/// </summary>
	/// <param name="window">Reference to the GLFW window that received the event</param>
	/// <param name="xPos">Mouse cursor x-position</param>
	/// <param name="yPos">Mouse cursor y-position</param>
	static void CursorCallback(GLFWwindow* window, double xPos, double yPos);

	/// <summary>
	/// GLFW callback function for when the mouse cursor entered or left the window
	/// </summary>
	/// <param name="window">Reference to the GLFW window that received the event</param>
	/// <param name="entered">Integer indicating whether the cursor entered (1) or left (0)</param>
	static void CursorEnterCallback(GLFWwindow* window, int entered);
};
