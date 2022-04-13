#include "Scenes/SandboxScene.hpp"

#include "Input.hpp"
#include "Window.hpp"
#include "WindowManager.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>

/**
 * @brief Constructor
 * @param[in] sceneManager Scene manager
 */
SandboxScene::SandboxScene(SceneManager *sceneManager)
    : BaseScene(sceneManager)
{
}

/**
 * @brief Destructor
 */
SandboxScene::~SandboxScene()
{
}

/**
 * @brief Initializes the scene.
 */
void SandboxScene::Init()
{
}

/**
 * @brief Updates the scene.
 * @param[in] deltaTime Time elapsed since the previous frame
 */
void SandboxScene::Update(const float &deltaTime)
{
}

/**
 * @brief Updates the scene with a fixed timestep.
 * @param[in] timestep Fixed timestep
 */
void SandboxScene::FixedUpdate(const float &timestep)
{
}

/**
 * @brief Draws the scene
 */
void SandboxScene::Draw()
{
}

/**
 * @brief Cleans up the resources used by the scene.
 */
void SandboxScene::Cleanup()
{
}
