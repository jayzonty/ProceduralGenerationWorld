// Quick note: GLAD needs to be included first before GLFW.
// Otherwise, GLAD will complain about gl.h being already included.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "Shader.hpp"

// ---------------
// Function declarations
// ---------------

/// <summary>
/// Function for handling the event when the size of the framebuffer changed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="width">New width</param>
/// <param name="height">New height</param>
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height);

/// <summary>
/// Function for handling when a key is pressed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="key">Key code</param>
/// <param name="scanCode">Scan code</param>
/// <param name="action">Action</param>
/// <param name="mods">Modifiers</param>
void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

/// <summary>
/// Struct containing data about a vertex
/// </summary>
struct Vertex
{
	GLfloat x, y, z;	// Position
	GLubyte r, g, b;	// Color
};

// Camera
Camera camera;

// Previous x and y positions of the cursor
double prevCursorX, prevCursorY;

/// <summary>
/// Main function.
/// </summary>
/// <returns>An integer indicating whether the program ended successfully or not.
/// A value of 0 indicates the program ended succesfully, while a non-zero value indicates
/// something wrong happened during execution.</returns>
int main()
{
	// Initialize GLFW
	int glfwInitStatus = glfwInit();
	if (glfwInitStatus == GLFW_FALSE)
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return 1;
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
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Hello Triangle", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Tell GLFW to use the OpenGL context that was assigned to the window that we just created
	glfwMakeContextCurrent(window);

	// Set the callback function for when the framebuffer size changed
	glfwSetFramebufferSizeCallback(window, FramebufferSizeChangedCallback);

	// Set the callback function for when a key was pressed
	glfwSetKeyCallback(window, KeyCallback);

	// Disable the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Tell GLAD to load the OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	// --- Vertex specification ---

	// Set up the data for each vertex of the triangle
	Vertex vertices[3];
	vertices[0].x = -0.5f;	vertices[0].y = -0.5f;	vertices[0].z = 0.0f;
	vertices[0].r = 255;	vertices[0].g = 0;		vertices[0].b = 0;

	vertices[1].x = 0.5f;	vertices[1].y = -0.5f;	vertices[1].z = 0.0f;
	vertices[1].r = 0;		vertices[1].g = 255;	vertices[1].b = 0;

	vertices[2].x = 0.0f;	vertices[2].y = 0.5f;	vertices[2].z = 0.0f;
	vertices[2].r = 0;		vertices[2].g = 0;		vertices[2].b = 255;

	// Create a vertex buffer object (VBO), and upload our vertices data to the VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create a vertex array object that contains data on how to map vertex attributes
	// (e.g., position, color) to vertex shader properties.
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Vertex attribute 0 - Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex attribute 1 - Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(sizeof(GLfloat) * 3));

	glBindVertexArray(0);

	// Create a shader program
	ShaderProgram shaderProgram;
	shaderProgram.InitFromFiles("main.vsh", "main.fsh");

	// Tell OpenGL the dimensions of the region where stuff will be drawn.
	// For now, tell OpenGL to use the whole screen
	glViewport(0, 0, windowWidth, windowHeight);

	// Initialize the previous cursor x and y values
	glfwGetCursorPos(window, &prevCursorX, &prevCursorY);

	camera.SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	camera.SetAspectRatio(static_cast<float>(windowWidth) / static_cast<float>(windowHeight));

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the colors in our off-screen framebuffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the shader program that we created
		shaderProgram.Use();

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		double cursorX, cursorY;
		glfwGetCursorPos(window, &cursorX, &cursorY);
		double cursorXDelta = cursorX - prevCursorX;
		double cursorYDelta = -(cursorY - prevCursorY);
		prevCursorX = cursorX;
		prevCursorY = cursorY;
		camera.SetYaw(camera.GetYaw() + static_cast<float>(cursorXDelta));
		camera.SetPitch(glm::clamp(camera.GetPitch() + static_cast<float>(cursorYDelta), -89.0f, 89.0f));

		float movementZ = 0.0f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			movementZ = 1.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			movementZ = -1.0f;
		}

		float movementX = 0.0f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			movementX = -1.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			movementX = 1.0f;
		}

		glm::vec3 movement = camera.GetForwardVector() * movementZ + camera.GetRightVector() * movementX;
		if (glm::length(movement) > 0.0f)
		{
			camera.SetPosition(camera.GetPosition() + movement * 0.1f);
		}

		shaderProgram.SetUniformMatrix4fv("projMatrix", false, glm::value_ptr(camera.GetProjectionMatrix()));
		shaderProgram.SetUniformMatrix4fv("viewMatrix", false, glm::value_ptr(camera.GetViewMatrix()));

		// Draw the 3 vertices using triangle primitives
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		// Unuse the shader program
		shaderProgram.Unuse();

		// Tell GLFW to swap the screen buffer with the offscreen buffer
		glfwSwapBuffers(window);

		// Tell GLFW to process window events (e.g., input events, window closed events, etc.)
		glfwPollEvents();
	}

	// --- Cleanup ---

	// Delete the VBO that contains our vertices
	glDeleteBuffers(1, &vbo);

	// Delete the vertex array object
	glDeleteVertexArrays(1, &vao);

	// Re-enable the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Remember to tell GLFW to clean itself up before exiting the application
	glfwTerminate();

	return 0;
}

/// <summary>
/// Function for handling the event when the size of the framebuffer changed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="width">New width</param>
/// <param name="height">New height</param>
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	// Whenever the size of the framebuffer changed (due to window resizing, etc.),
	// update the dimensions of the region to the new size
	glViewport(0, 0, width, height);
}

/// <summary>
/// Function for handling when a key is pressed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="key">Key code</param>
/// <param name="scanCode">Scan code</param>
/// <param name="action">Action</param>
/// <param name="mods">Modifiers</param>
void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
