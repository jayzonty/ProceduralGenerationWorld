#include "MainScene.hpp"

#include "Constants.hpp"
#include "Input.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

/// <summary>
/// Constructor
/// </summary>
MainScene::MainScene()
	: SceneBase()
	, m_debugInfoTextFont()
	, m_debugInfoTextDisplay()
	, m_crossHairTextDisplay()
	, m_sceneShaderProgram()
	, m_blocksTexture()
	, m_camera()
	, m_world(nullptr)
	, m_prevChunkIndices()
	, m_chunkRenderDistance(8)
	, m_skyColor()
{
}

/// <summary>
/// Destructor
/// </summary>
MainScene::~MainScene()
{
}

/// <summary>
/// Start scene
/// </summary>
void MainScene::Start()
{
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable face culling
	glEnable(GL_CULL_FACE);

	// Set sky color
	m_skyColor = glm::vec4(0.678f, 0.847f, 0.902f, 1.0f);
	glClearColor(m_skyColor.r, m_skyColor.g, m_skyColor.b, m_skyColor.a);

	// Initialize the scene shader
	m_sceneShaderProgram.InitFromFiles("main_lighting.vsh", "main_lighting.fsh");

	// Initialize the font to be used for the debug info text
	m_debugInfoTextFont.Load("Resources/Fonts/SourceCodePro/SourceCodePro-Regular.ttf");
	m_debugInfoTextFont.SetSize(14);
	if (!m_debugInfoTextFont.IsLoaded())
	{
		std::cerr << "Failed to load font" << std::endl;
	}

	// Initialize debug info text display
	m_debugInfoTextDisplay.SetFont(m_debugInfoTextFont);
	m_debugInfoTextDisplay.SetColor(glm::vec4(255.0f));
	m_debugInfoTextDisplay.SetLineSpacing(2.0f);

	m_crossHairTextDisplay.SetFont(m_debugInfoTextFont);
	m_crossHairTextDisplay.SetColor(glm::vec4(255.0f));
	m_crossHairTextDisplay.SetString("+");
	int crosshairTextWidth, crosshairTextHeight;
	m_crossHairTextDisplay.ComputeSize(&crosshairTextWidth, &crosshairTextHeight);
	m_crossHairTextDisplay.SetPosition(400.0f - crosshairTextWidth / 2, 300.0f - crosshairTextHeight / 2);

	// Create blocks texture
	m_blocksTexture.CreateFromFile("Resources/Textures/Blocks.png");

	// Setup camera
	m_camera.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	m_camera.SetAspectRatio(800.0f / 600.0f);

	// Create world data
	m_world = new World();

	// Generate initial chunks
	for (int x = m_prevChunkIndices.x - m_chunkRenderDistance; x <= m_prevChunkIndices.x + m_chunkRenderDistance; ++x)
	{
		for (int z = m_prevChunkIndices.z - m_chunkRenderDistance; z <= m_prevChunkIndices.z + m_chunkRenderDistance; ++z)
		{
			m_world->GenerateChunkAt(x, z);
		}
	}
}

/// <summary>
/// Finish scene
/// </summary>
void MainScene::Finish()
{
}

/// <summary>
/// Updates scene state
/// </summary>
/// <param name="deltaTime"></param>
void MainScene::Update(float deltaTime)
{
	int cursorXDelta, cursorYDelta;
	Input::GetMouseDelta(&cursorXDelta, &cursorYDelta);

	m_camera.SetYaw(m_camera.GetYaw() + static_cast<float>(cursorXDelta) * 0.5f);
	m_camera.SetPitch(glm::clamp(m_camera.GetPitch() - static_cast<float>(cursorYDelta) * 0.5f, -89.0f, 89.0f));

	float movementZ = 0.0f;
	if (Input::IsDown(Input::Key::W))
	{
		movementZ = 1.0f;
	}
	else if (Input::IsDown(Input::Key::S))
	{
		movementZ = -1.0f;
	}

	float movementX = 0.0f;
	if (Input::IsDown(Input::Key::A))
	{
		movementX = -1.0f;
	}
	else if (Input::IsDown(Input::Key::D))
	{
		movementX = 1.0f;
	}

	float movementSpeed = 10.0f;
	glm::vec3 movement = m_camera.GetForwardVector() * movementZ + m_camera.GetRightVector() * movementX;
	if (glm::length(movement) > 0.0f)
	{
		m_camera.SetPosition(m_camera.GetPosition() + movement * movementSpeed * deltaTime);
	}

	int currentChunkX = static_cast<int>(glm::floor(m_camera.GetPosition().x / Constants::BLOCK_SIZE / Constants::CHUNK_WIDTH));
	int currentChunkZ = static_cast<int>(glm::floor(m_camera.GetPosition().z / Constants::BLOCK_SIZE / Constants::CHUNK_DEPTH));

	if ((currentChunkX != m_prevChunkIndices.x) || (currentChunkZ != m_prevChunkIndices.z))
	{
		for (int x = currentChunkX - m_chunkRenderDistance; x <= currentChunkX + m_chunkRenderDistance; ++x)
		{
			for (int z = currentChunkZ - m_chunkRenderDistance; z <= currentChunkZ + m_chunkRenderDistance; ++z)
			{
				if (m_world->GetChunkAt(x, z) == nullptr)
				{
					m_world->GenerateChunkAt(x, z);
				}
			}
		}

		m_prevChunkIndices.x = currentChunkX;
		m_prevChunkIndices.z = currentChunkZ;
	}

	Ray ray(m_camera.GetPosition(), m_camera.GetForwardVector());
	Block* raycastBlock = m_world->Raycast(ray, 5.0f);

	if (Input::IsPressed(Input::Button::LEFT_MOUSE))
	{
		if (raycastBlock != nullptr)
		{
			glm::ivec3 blockPositionInChunk = raycastBlock->GetPositionInChunk();
			Chunk* chunk = m_world->GetChunkAtWorldPosition(raycastBlock->GetPositionInWorld());
			chunk->SetBlockAt(blockPositionInChunk.x, blockPositionInChunk.y, blockPositionInChunk.z, nullptr);
			chunk->GenerateMesh();
		}
	}
}

/// <summary>
/// Draws the scene
/// </summary>
void MainScene::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_sceneShaderProgram.Use();

	m_sceneShaderProgram.SetUniformMatrix4fv("projMatrix", false, glm::value_ptr(m_camera.GetProjectionMatrix()));
	m_sceneShaderProgram.SetUniformMatrix4fv("viewMatrix", false, glm::value_ptr(m_camera.GetViewMatrix()));

	glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);

	glm::vec3 lightAmbient(0.1f, 0.1f, 0.1f);
	glm::vec3 lightDiffuse(1.0f, 1.0f, 1.0f);

	glm::vec3 materialAmbient(1.0f, 1.0f, 1.0f);
	glm::vec3 materialDiffuse(1.0f, 1.0f, 1.0f);

	m_sceneShaderProgram.SetUniform3f("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
	m_sceneShaderProgram.SetUniform3f("light.ambient", lightAmbient.x, lightAmbient.y, lightAmbient.z);
	m_sceneShaderProgram.SetUniform3f("light.diffuse", lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
	m_sceneShaderProgram.SetUniform3f("material.ambient", materialAmbient.x, materialAmbient.y, materialAmbient.z);
	m_sceneShaderProgram.SetUniform3f("material.diffuse", materialDiffuse.x, materialDiffuse.y, materialDiffuse.z);

	m_sceneShaderProgram.SetUniform4f("skyColor", m_skyColor.r, m_skyColor.g, m_skyColor.b, m_skyColor.a);
	m_sceneShaderProgram.SetUniform1f("fogGradient", 1.5f);
	m_sceneShaderProgram.SetUniform1f("fogDensity", 0.01f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_blocksTexture.GetHandle());
	m_sceneShaderProgram.SetUniform1i("tex", 0);

	m_world->Draw();

	int currentChunkX = static_cast<int>(glm::floor(m_camera.GetPosition().x / Constants::BLOCK_SIZE / Constants::CHUNK_WIDTH));
	int currentChunkZ = static_cast<int>(glm::floor(m_camera.GetPosition().z / Constants::BLOCK_SIZE / Constants::CHUNK_DEPTH));

	std::stringstream displayStringStream;
	displayStringStream << "Chunk: " << currentChunkX << "  " << currentChunkZ << std::endl;
	displayStringStream << "Position: " << std::fixed << std::setprecision(2) << m_camera.GetPosition().x << "  " << m_camera.GetPosition().y << "  " << m_camera.GetPosition().z << std::endl;

	Block* currentBlock = m_world->GetBlockAtWorldPosition(m_camera.GetPosition());
	if (currentBlock != nullptr)
	{
		glm::ivec3 currentBlockPosition = currentBlock->GetPositionInWorld();
		displayStringStream << "Current block: " << currentBlockPosition.x << " " << currentBlockPosition.y << " " << currentBlockPosition.z << std::endl;
	}

	Ray ray(m_camera.GetPosition(), m_camera.GetForwardVector());
	Block* raycastBlock = m_world->Raycast(ray, 5.0f);
	if (raycastBlock != nullptr)
	{
		glm::ivec3 raycastBlockPosition = raycastBlock->GetPositionInWorld();
		displayStringStream << "Looking at block: " << raycastBlockPosition.x << " " << raycastBlockPosition.y << " " << raycastBlockPosition.z << std::endl;
	}

	m_debugInfoTextDisplay.SetString(displayStringStream.str());
	int windowWidth = 800, windowHeight = 600; // TODO: Have a way to access window size
	int textWidth, textHeight;
	m_debugInfoTextDisplay.ComputeSize(&textWidth, &textHeight);
	m_debugInfoTextDisplay.SetPosition(0.0f, (windowHeight - textHeight) * 1.0f);
	m_debugInfoTextDisplay.Draw(glm::ortho(0.0f, windowWidth * 1.0f, 0.0f, windowHeight * 1.0f));

	m_crossHairTextDisplay.Draw(glm::ortho(0.0f, windowWidth * 1.0f, 0.0f, windowHeight * 1.0f));

	m_sceneShaderProgram.Unuse();
}
