#include "Scenes/MainScene.hpp"

#include "Scenes/BaseScene.hpp"
#include "Constants.hpp"
#include "Input.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"
#include "ShaderProgram.hpp"

#include "Enums/BlockTypeEnum.hpp"
#include "EntityTemplates/BlockTemplateManager.hpp"
#include "Utils/MathUtils.hpp"
#include "Window.hpp"
#include "WindowManager.hpp"
#include "WorldGenParams.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>

/**
 * @brief Constructor
 */
MainScene::MainScene(SceneManager *sceneManager)
	: BaseScene(sceneManager)
	, m_camera()
	, m_world(nullptr)
	, m_prevChunkIndices()
	, m_chunkRenderDistance(8)
	, m_currentTime(0)
	, m_timeTickTimer(0.0f)
	, m_uiRenderer()
	, m_batchRenderer()
	, m_sunMesh()
	, m_moonMesh()
{
}

/**
 * @brief Destructor
 */
MainScene::~MainScene()
{
}

/**
 * @brief Initializes the scene.
 */
void MainScene::Init()
{
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable face culling
	glEnable(GL_CULL_FACE);

	// Initialize the scene shader
	ResourceManager::GetInstance().CreateShader("Resources/Shaders/Main.vsh", "Resources/Shaders/Main.fsh", "main");
	ResourceManager::GetInstance().CreateShader("Resources/Shaders/SunMoon.vsh", "Resources/Shaders/SunMoon.fsh", "sun_moon");
	ResourceManager::GetInstance().CreateShader("Resources/Shaders/Water.vsh", "Resources/Shaders/Water.fsh", "water");

	// --- Initialize block templates ---
	float uvSize = 32.0f / 256.0f;
	{
		BlockTemplate dirtBlockTemplate;
		dirtBlockTemplate.SetFaceUVRect(BlockFaceEnum::TOP, glm::vec4(0.0f / 256.0f, 224.0f / 256.0f, uvSize, uvSize));
		dirtBlockTemplate.SetFaceUVRect(BlockFaceEnum::BOTTOM, glm::vec4(64.0f / 256.0f, 224.0f / 256.0f, uvSize, uvSize));
		dirtBlockTemplate.SetFaceUVRect(BlockFaceEnum::LEFT, glm::vec4(32.0f / 256.0f, 224.0f / 256.0f, uvSize, uvSize));
		dirtBlockTemplate.SetFaceUVRect(BlockFaceEnum::RIGHT, glm::vec4(32.0f / 256.0f, 224.0f / 256.0f, uvSize, uvSize));
		dirtBlockTemplate.SetFaceUVRect(BlockFaceEnum::FRONT, glm::vec4(32.0f / 256.0f, 224.0f / 256.0f, uvSize, uvSize));
		dirtBlockTemplate.SetFaceUVRect(BlockFaceEnum::BACK, glm::vec4(32.0f / 256.0f, 224.0f / 256.0f, uvSize, uvSize));
		BlockTemplateManager::GetInstance().AddBlockTemplate(BlockTypeEnum::DIRT, dirtBlockTemplate);
	}
	{
		BlockTemplate stoneBlockTemplate;
		stoneBlockTemplate.SetFaceUVRect(BlockFaceEnum::TOP, glm::vec4(0.0f / 256.0f, 192.0f / 256.0f, uvSize, uvSize));
		stoneBlockTemplate.SetFaceUVRect(BlockFaceEnum::BOTTOM, glm::vec4(0.0f / 256.0f, 192.0f / 256.0f, uvSize, uvSize));
		stoneBlockTemplate.SetFaceUVRect(BlockFaceEnum::LEFT, glm::vec4(0.0f / 256.0f, 192.0f / 256.0f, uvSize, uvSize));
		stoneBlockTemplate.SetFaceUVRect(BlockFaceEnum::RIGHT, glm::vec4(0.0f / 256.0f, 192.0f / 256.0f, uvSize, uvSize));
		stoneBlockTemplate.SetFaceUVRect(BlockFaceEnum::FRONT, glm::vec4(0.0f / 256.0f, 192.0f / 256.0f, uvSize, uvSize));
		stoneBlockTemplate.SetFaceUVRect(BlockFaceEnum::BACK, glm::vec4(0.0f / 256.0f, 192.0f / 256.0f, uvSize, uvSize));
		BlockTemplateManager::GetInstance().AddBlockTemplate(BlockTypeEnum::STONE, stoneBlockTemplate);
	}
	{
		BlockTemplate sandBlockTemplate;
		sandBlockTemplate.SetFaceUVRect(BlockFaceEnum::TOP, glm::vec4(0.0f / 256.0f, 160.0f / 256.0f, uvSize, uvSize));
		sandBlockTemplate.SetFaceUVRect(BlockFaceEnum::BOTTOM, glm::vec4(0.0f / 256.0f, 160.0f / 256.0f, uvSize, uvSize));
		sandBlockTemplate.SetFaceUVRect(BlockFaceEnum::LEFT, glm::vec4(0.0f / 256.0f, 160.0f / 256.0f, uvSize, uvSize));
		sandBlockTemplate.SetFaceUVRect(BlockFaceEnum::RIGHT, glm::vec4(0.0f / 256.0f, 160.0f / 256.0f, uvSize, uvSize));
		sandBlockTemplate.SetFaceUVRect(BlockFaceEnum::FRONT, glm::vec4(0.0f / 256.0f, 160.0f / 256.0f, uvSize, uvSize));
		sandBlockTemplate.SetFaceUVRect(BlockFaceEnum::BACK, glm::vec4(0.0f / 256.0f, 160.0f / 256.0f, uvSize, uvSize));
		BlockTemplateManager::GetInstance().AddBlockTemplate(BlockTypeEnum::SAND, sandBlockTemplate);
	}

	// Create blocks texture
	ResourceManager::GetInstance().CreateTexture("Resources/Textures/Blocks.png", "blocks");
	// Create moon texture
	ResourceManager::GetInstance().CreateTexture("Resources/Textures/Moon.png", "moon");

	// Create world data
	m_world = new World();

	WorldGenParams worldGenParams;
	worldGenParams.worldSize = 1024;
	worldGenParams.worldMaxHeight = 30;

	worldGenParams.seed = 0;
	worldGenParams.noiseNumOctaves = 1;
	worldGenParams.noiseScale = 1.0f;
	worldGenParams.noisePersistence = 1.0f;
	worldGenParams.noiseLacunarity = 2.0f;
	m_world->SetWorldGenParams(worldGenParams);

	// Setup camera
	m_camera.SetPosition({worldGenParams.worldSize / 2.0f, worldGenParams.worldMaxHeight + 1.0f, worldGenParams.worldSize / 2.0f});
	m_camera.SetAspectRatio(800.0f / 600.0f);

	// Generate initial chunks
	m_world->LoadChunksWithinArea(m_prevChunkIndices, m_chunkRenderDistance);

	m_uiRenderer.Initialize(1000);
	m_batchRenderer.Initialize(10000, 100000);

	m_currentTime = 0;
	m_timeTickTimer = Constants::TIME_TICK_DURATION;

	m_sunMesh.vertices.emplace_back();
	m_sunMesh.vertices.back().position = { -1.0f, -1.0f,  0.0f };
	m_sunMesh.vertices.back().normal = { 0.0f, 0.0f, 1.0f };
	m_sunMesh.vertices.back().color = Constants::SUN_COLOR;
	m_sunMesh.vertices.back().uv = { 0.0f, 0.0f };
	m_sunMesh.vertices.emplace_back();
	m_sunMesh.vertices.back().position = {  1.0f, -1.0f, 0.0f };
	m_sunMesh.vertices.back().normal = { 0.0f, 0.0f, 1.0f };
	m_sunMesh.vertices.back().color = Constants::SUN_COLOR;
	m_sunMesh.vertices.back().uv = { 1.0f, 0.0f };
	m_sunMesh.vertices.emplace_back();
	m_sunMesh.vertices.back().position = {  1.0f,  1.0f, 0.0f };
	m_sunMesh.vertices.back().normal = { 0.0f, 0.0f, 1.0f };
	m_sunMesh.vertices.back().color = Constants::SUN_COLOR;
	m_sunMesh.vertices.back().uv = { 1.0f, 1.0f };
	m_sunMesh.vertices.emplace_back();
	m_sunMesh.vertices.back().position = { -1.0f,  1.0f,  0.0f };
	m_sunMesh.vertices.back().normal = { 0.0f, 0.0f, 1.0f };
	m_sunMesh.vertices.back().color = Constants::SUN_COLOR;
	m_sunMesh.vertices.back().uv = { 0.0f, 1.0f };
	m_sunMesh.indices.push_back(0);
	m_sunMesh.indices.push_back(1);
	m_sunMesh.indices.push_back(2);
	m_sunMesh.indices.push_back(2);
	m_sunMesh.indices.push_back(3);
	m_sunMesh.indices.push_back(0);

	m_moonMesh = m_sunMesh;
	m_moonMesh.vertices[0].color = Constants::MOON_COLOR;
	m_moonMesh.vertices[1].color = Constants::MOON_COLOR;
	m_moonMesh.vertices[2].color = Constants::MOON_COLOR;
	m_moonMesh.vertices[3].color = Constants::MOON_COLOR;
}

/**
 * @brief Updates scene state
 * @param[in] deltaTime Time elapsed since the previous frame
 */
void MainScene::Update(const float &deltaTime)
{
	m_timeTickTimer -= deltaTime;
	if (deltaTime > 0.0f)
	{
		while (m_timeTickTimer <= 0.0f)
		{
			m_currentTime = (m_currentTime + 1) % Constants::TOTAL_TIME_TICKS;
			m_timeTickTimer += Constants::TIME_TICK_DURATION;
		}
	}

	int cursorXDelta, cursorYDelta;
	Input::GetMouseDelta(&cursorXDelta, &cursorYDelta);

	m_camera.SetYaw(m_camera.GetYaw() + static_cast<float>(cursorXDelta) * 0.5f);
	m_camera.SetPitch(glm::clamp(m_camera.GetPitch() - static_cast<float>(cursorYDelta) * 0.5f, -89.0f, 89.0f));

	float movementZ = 0.0f;
	if (Input::IsKeyDown(Input::Key::W))
	{
		movementZ = 1.0f;
	}
	else if (Input::IsKeyDown(Input::Key::S))
	{
		movementZ = -1.0f;
	}

	float movementX = 0.0f;
	if (Input::IsKeyDown(Input::Key::A))
	{
		movementX = -1.0f;
	}
	else if (Input::IsKeyDown(Input::Key::D))
	{
		movementX = 1.0f;
	}

	float movementSpeed = 10.0f;
	glm::vec3 movement = m_camera.GetForwardVector() * movementZ + m_camera.GetRightVector() * movementX;
	movement = glm::normalize(movement);
	if (glm::length(movement) > 0.0f)
	{
		m_camera.SetPosition(m_camera.GetPosition() + movement * movementSpeed * deltaTime);
	}

	int currentChunkX = static_cast<int>(glm::floor(m_camera.GetPosition().x / Constants::BLOCK_SIZE / Constants::CHUNK_WIDTH));
	int currentChunkZ = static_cast<int>(glm::floor(m_camera.GetPosition().z / Constants::BLOCK_SIZE / Constants::CHUNK_DEPTH));

	if ((currentChunkX != m_prevChunkIndices.x) || (currentChunkZ != m_prevChunkIndices.z))
	{
		m_world->LoadChunksWithinArea(glm::ivec3(currentChunkX, 0, currentChunkZ), m_chunkRenderDistance);
		m_world->UnloadChunksOutsideArea(glm::ivec3(currentChunkX, 0, currentChunkZ), m_chunkRenderDistance);

		m_prevChunkIndices.x = currentChunkX;
		m_prevChunkIndices.z = currentChunkZ;
	}

	Ray ray(m_camera.GetPosition(), m_camera.GetForwardVector());
	Block* raycastBlock = m_world->Raycast(ray, 5.0f);

	if (Input::IsMouseButtonPressed(Input::Button::LEFT_MOUSE))
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

/**
 * @brief Updates the scene with a fixed timestep.
 * @param[in] timestep Fixed timestep
 */
void MainScene::FixedUpdate(const float &timestep)
{
}

/**
 * @brief Draws the scene
 */
void MainScene::Draw()
{
	glEnable(GL_DEPTH_TEST);

	float sunLightStrength = 1.0f;
	float moonLightStrength = 0.0f;
	glm::vec4 skyColor(0.0f);
	if (m_currentTime < Constants::DUSK_START_TIME)
	{
		skyColor = Constants::DAY_SKY_COLOR;
		sunLightStrength = 1.0f; moonLightStrength = 0.0f;
	}
	else if (m_currentTime < Constants::NIGHT_START_TIME)
	{
		const uint32_t duskDuration = Constants::NIGHT_START_TIME - Constants::DUSK_START_TIME;

		// t goes from 0 -> 1
		float t = (m_currentTime - Constants::DUSK_START_TIME) * 1.0f / duskDuration;
		skyColor = MathUtils::Lerp(Constants::DAY_SKY_COLOR, Constants::NIGHT_SKY_COLOR, t);

		sunLightStrength = MathUtils::Lerp(1.0f, 0.0f, t);
		moonLightStrength = MathUtils::Lerp(0.0f, 1.0f, t);
	}
	else if (m_currentTime < Constants::DAWN_START_TIME)
	{
		skyColor = Constants::NIGHT_SKY_COLOR;
		sunLightStrength = 0.0f; moonLightStrength = 1.0f;
	}
	else
	{
		const uint32_t dawnDuration = Constants::TOTAL_TIME_TICKS - Constants::DAWN_START_TIME;

		// t goes from 1 -> 0
		float t = (Constants::TOTAL_TIME_TICKS - m_currentTime) * 1.0f / dawnDuration;
		skyColor = MathUtils::Lerp(Constants::DAY_SKY_COLOR, Constants::NIGHT_SKY_COLOR, t);

		sunLightStrength = MathUtils::Lerp(1.0f, 0.0f, t);
		moonLightStrength = MathUtils::Lerp(0.0f, 1.0f, t);
	}

	glClearColor(skyColor.r, skyColor.g, skyColor.b, skyColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw terrain
	ShaderProgram* mainShader = ResourceManager::GetInstance().GetShader("main");
	mainShader->Use();

	mainShader->SetUniformMatrix4fv("projMatrix", false, glm::value_ptr(m_camera.GetProjectionMatrix()));
	mainShader->SetUniformMatrix4fv("viewMatrix", false, glm::value_ptr(m_camera.GetViewMatrix()));

	glm::vec3 sunLightDirection(0.0f);
	float timeProgress = m_currentTime * 1.0f / Constants::TOTAL_TIME_TICKS;
	sunLightDirection.x = glm::cos(glm::radians(timeProgress * 360.0f));
	sunLightDirection.y = -glm::sin(glm::radians(timeProgress * 360.0f));
	sunLightDirection = glm::normalize(sunLightDirection);

	glm::vec3 moonLightDirection = -sunLightDirection;

	glm::vec3 sunLightAmbient(0.05f, 0.05f, 0.05f);
	sunLightAmbient *= sunLightStrength;
	glm::vec3 sunLightDiffuse(1.0f, 1.0f, 1.0f);
	sunLightDiffuse *= sunLightStrength;
	glm::vec3 moonLightAmbient(0.05f, 0.05f, 0.05f);
	moonLightAmbient *= moonLightStrength;
	glm::vec3 moonLightDiffuse(0.25f, 0.25f, 0.25f);
	moonLightDiffuse *= moonLightStrength;

	glm::vec3 materialAmbient(1.0f, 1.0f, 1.0f);
	glm::vec3 materialDiffuse(1.0f, 1.0f, 1.0f);

	mainShader->SetUniform3f("lights[0].direction", sunLightDirection.x, sunLightDirection.y, sunLightDirection.z);
	mainShader->SetUniform3f("lights[0].ambient", sunLightAmbient.x, sunLightAmbient.y, sunLightAmbient.z);
	mainShader->SetUniform3f("lights[0].diffuse", sunLightDiffuse.x, sunLightDiffuse.y, sunLightDiffuse.z);
	mainShader->SetUniform3f("lights[1].direction", moonLightDirection.x, moonLightDirection.y, moonLightDirection.z);
	mainShader->SetUniform3f("lights[1].ambient", moonLightAmbient.x, moonLightAmbient.y, moonLightAmbient.z);
	mainShader->SetUniform3f("lights[1].diffuse", moonLightDiffuse.x, moonLightDiffuse.y, moonLightDiffuse.z);

	mainShader->SetUniform3f("material.ambient", materialAmbient.x, materialAmbient.y, materialAmbient.z);
	mainShader->SetUniform3f("material.diffuse", materialDiffuse.x, materialDiffuse.y, materialDiffuse.z);

	mainShader->SetUniform4f("skyColor", skyColor.r, skyColor.g, skyColor.b, skyColor.a);
	mainShader->SetUniform1f("fogGradient", 1.5f);
	mainShader->SetUniform1f("fogDensity", 0.01f);

	Texture* blocksTexture = ResourceManager::GetInstance().GetTexture("blocks");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blocksTexture->GetHandle());
	mainShader->SetUniform1i("tex", 0);

	m_world->DrawTerrainMeshes();

	// Draw water mesh
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec4 waterColor(0.0f, 0.0f, 0.45f, 0.35f);
	ShaderProgram* waterShader = ResourceManager::GetInstance().GetShader("water");
	waterShader->Use();
	waterShader->SetUniformMatrix4fv("projMatrix", false, glm::value_ptr(m_camera.GetProjectionMatrix()));
	waterShader->SetUniformMatrix4fv("viewMatrix", false, glm::value_ptr(m_camera.GetViewMatrix()));
	waterShader->SetUniform4f("skyColor", skyColor.r, skyColor.g, skyColor.b, skyColor.a);
	waterShader->SetUniform1f("fogGradient", 1.5f);
	waterShader->SetUniform1f("fogDensity", 0.01f);
	waterShader->SetUniform4f("waterColor", waterColor.r, waterColor.g, waterColor.b, waterColor.a);

	m_world->DrawWaterMeshes();
	
	waterShader->Unuse();

	int currentChunkX = static_cast<int>(glm::floor(m_camera.GetPosition().x / Constants::BLOCK_SIZE / Constants::CHUNK_WIDTH));
	int currentChunkZ = static_cast<int>(glm::floor(m_camera.GetPosition().z / Constants::BLOCK_SIZE / Constants::CHUNK_DEPTH));

	std::cout << "Looking at direction: " << m_camera.GetForwardVector().x << "," << m_camera.GetForwardVector().y << "," << m_camera.GetForwardVector().z << std::endl;

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

	ResourceManager::GetInstance().GetShader("main")->Unuse();

	Window *mainWindow = WindowManager::GetMainWindow();
	glm::mat4 uiCameraMatrix = glm::ortho(0.0f, mainWindow->GetWidth() * 1.0f, 0.0f, mainWindow->GetHeight() * 1.0f);

	// Draw sun
	m_batchRenderer.Begin();
	m_batchRenderer.AddMesh(m_sunMesh);
	m_batchRenderer.End();

	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, -sunLightDirection);
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(timeProgress * 360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(0.25f, 0.25f, 0.25f));
	transform = m_camera.GetProjectionMatrix() * glm::mat4(glm::mat3(m_camera.GetViewMatrix())) * transform;

	ShaderProgram* sunMoonShader = ResourceManager::GetInstance().GetShader("sun_moon");
	sunMoonShader->Use();
	sunMoonShader->SetUniformMatrix4fv("mvpMatrix", false, glm::value_ptr(transform));
	sunMoonShader->SetUniform1f("innerRadius", 0.6f);
	sunMoonShader->SetUniform1f("visibility", 1.0f);
	glDepthFunc(GL_LEQUAL);
	m_batchRenderer.Render();

	// Draw moon
	m_batchRenderer.Begin();
	m_batchRenderer.AddMesh(m_moonMesh, glm::mat4(1.0f), 0, 0, ResourceManager::GetInstance().GetTexture("moon")->GetHandle());
	m_batchRenderer.End();

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, -moonLightDirection);
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(timeProgress * 360.0f + 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(0.15f, 0.15f, 0.15f));
	transform = m_camera.GetProjectionMatrix() * glm::mat4(glm::mat3(m_camera.GetViewMatrix())) * transform;
	sunMoonShader->SetUniformMatrix4fv("mvpMatrix", false, glm::value_ptr(transform));
	sunMoonShader->SetUniform1f("innerRadius", 0.8f);
	sunMoonShader->SetUniform1f("visibility", moonLightStrength);
	m_batchRenderer.Render();
	glDepthFunc(GL_LESS);

	sunMoonShader->Unuse();

	/*m_uiRenderer.Begin();
	m_uiRenderer.DrawQuad({ 10.0f, 10.0f }, { 256.0f, 256.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0, 0, m_noiseTexture.GetHandle(), { 0.0f, 0.0f, 1.0f, 1.0f });
	m_uiRenderer.End();
	m_uiRenderer.Render(uiCameraMatrix);*/
	glDisable(GL_BLEND);
}

/**
 * @brief Cleans up the resources used by the scene.
 */
void MainScene::Cleanup()
{
	m_uiRenderer.Cleanup();

	if (m_world != nullptr)
	{
		delete m_world;
		m_world = nullptr;
	}
}
