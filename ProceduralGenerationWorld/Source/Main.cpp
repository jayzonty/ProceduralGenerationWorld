// Quick note: GLAD needs to be included first before GLFW.
// Otherwise, GLAD will complain about gl.h being already included.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <FastNoiseLite/FastNoiseLite.h>

#include "Camera.hpp"
#include "Chunk.hpp"
#include "Constants.hpp"
#include "Font.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Text.hpp"
#include "World.hpp"

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

	// Create a shader program
	ShaderProgram shaderProgram;
	shaderProgram.InitFromFiles("main.vsh", "main.fsh");

	// Set up the data for the mesh
	Mesh mesh;

	std::vector<glm::vec3> vertexPositions;
	vertexPositions.emplace_back(-1.0f, -1.0f, 0.0f);
	vertexPositions.emplace_back(1.0f, -1.0f, 0.0f);
	vertexPositions.emplace_back(0.0f, 1.0f, 0.0f);

	std::vector<glm::vec4> vertexColors;
	vertexColors.emplace_back(1.0f, 0.0f, 0.0f, 1.0f);
	vertexColors.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
	vertexColors.emplace_back(0.0f, 0.0f, 1.0f, 1.0f);

	std::vector<GLuint> indices;
	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);

	mesh.SetVertexPositions(vertexPositions);
	mesh.SetVertexColors(vertexColors);
	mesh.SetIndices(indices);

	mesh.GenerateMesh();

	Font font;
	font.Load("Resources/Fonts/SourceCodePro/SourceCodePro-Regular.ttf");
	font.SetSize(14);
	if (!font.IsLoaded())
	{
		std::cerr << "Failed to load font" << std::endl;
	}

	Text text;
	text.SetFont(font);
	text.SetColor(glm::vec4(255.0f));
	text.SetLineSpacing(2.0f);

	// Tell OpenGL the dimensions of the region where stuff will be drawn.
	// For now, tell OpenGL to use the whole screen
	glViewport(0, 0, windowWidth, windowHeight);

	// Initialize the previous cursor x and y values
	glfwGetCursorPos(window, &prevCursorX, &prevCursorY);

	camera.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	camera.SetAspectRatio(static_cast<float>(windowWidth) / static_cast<float>(windowHeight));

	World* world = new World();

	int prevChunkX = 0, prevChunkZ = 0;
	int chunkDistance = 4;

	for (int x = prevChunkX - chunkDistance; x <= prevChunkX + chunkDistance; ++x)
	{
		for (int z = prevChunkZ - chunkDistance; z <= prevChunkZ + chunkDistance; ++z)
		{
			world->GenerateChunkAt(x, z);
		}
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	double prevTime = glfwGetTime();

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// For now, just use a basic delta time calculation
		double deltaTimeAsDouble = glfwGetTime() - prevTime;
		prevTime = glfwGetTime();
		float deltaTime = static_cast<float>(deltaTimeAsDouble);

		// Clear the colors in our off-screen framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double cursorX, cursorY;
		glfwGetCursorPos(window, &cursorX, &cursorY);
		double cursorXDelta = cursorX - prevCursorX;
		double cursorYDelta = -(cursorY - prevCursorY);
		prevCursorX = cursorX;
		prevCursorY = cursorY;
		camera.SetYaw(camera.GetYaw() + static_cast<float>(cursorXDelta) * 0.5f);
		camera.SetPitch(glm::clamp(camera.GetPitch() + static_cast<float>(cursorYDelta) * 0.5f, -89.0f, 89.0f));

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

		float movementSpeed = 10.0f;
		glm::vec3 movement = camera.GetForwardVector() * movementZ + camera.GetRightVector() * movementX;
		if (glm::length(movement) > 0.0f)
		{
			camera.SetPosition(camera.GetPosition() + movement * movementSpeed * deltaTime);
		}

		int currentChunkX = static_cast<int>(glm::floor(camera.GetPosition().x / Constants::BLOCK_SIZE / Constants::CHUNK_WIDTH));
		int currentChunkZ = static_cast<int>(glm::floor(camera.GetPosition().z / Constants::BLOCK_SIZE / Constants::CHUNK_DEPTH));

		if ((currentChunkX != prevChunkX) || (currentChunkZ != prevChunkZ))
		{
			for (int x = currentChunkX - chunkDistance; x <= currentChunkX + chunkDistance; ++x)
			{
				for (int z = currentChunkZ - chunkDistance; z <= currentChunkZ + chunkDistance; ++z)
				{
					if (world->GetChunkAt(x, z) == nullptr)
					{
						world->GenerateChunkAt(x, z);
					}
				}
			}

			prevChunkX = currentChunkX;
			prevChunkZ = currentChunkZ;
		}

		shaderProgram.Use();

		shaderProgram.SetUniformMatrix4fv("projMatrix", false, glm::value_ptr(camera.GetProjectionMatrix()));
		shaderProgram.SetUniformMatrix4fv("viewMatrix", false, glm::value_ptr(camera.GetViewMatrix()));

		world->Draw();

		Block* currentBlock = world->GetBlockAtWorldPosition(camera.GetPosition());
		glm::ivec3 currentBlockPosition = currentBlock->GetPositionInWorld();

		std::stringstream displayStringStream;
		displayStringStream << "Chunk: " << currentChunkX << " (*) " << currentChunkZ << std::endl;
		displayStringStream << "Position: " << std::fixed << std::setprecision(2) << camera.GetPosition().x << "  " << camera.GetPosition().y << "  " << camera.GetPosition().z << std::endl;
		displayStringStream << "Current block: " << currentBlockPosition.x << " " << currentBlockPosition.y << " " << currentBlockPosition.z << std::endl;
		text.SetString(displayStringStream.str());
		int textWidth, textHeight;
		text.ComputeSize(&textWidth, &textHeight);
		text.SetPosition(0.0f, (windowHeight - textHeight) * 1.0f);
		text.Draw(glm::ortho(0.0f, windowWidth * 1.0f, 0.0f, windowHeight * 1.0f));

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		// Unuse the shader program
		shaderProgram.Unuse();

		// Tell GLFW to swap the screen buffer with the offscreen buffer
		glfwSwapBuffers(window);

		// Tell GLFW to process window events (e.g., input events, window closed events, etc.)
		glfwPollEvents();
	}

	delete world;

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
