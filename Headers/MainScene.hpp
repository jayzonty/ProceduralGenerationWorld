#pragma once

#include "Engine/BaseScene.hpp"
#include "Engine/Graphics/Texture.hpp"

#include "Camera.hpp"
//#include "Font.hpp"
//#include "Text.hpp"
#include "World.hpp"

#include <glm/glm.hpp>

/**
 * Main scene class
 */
class MainScene : public Engine::BaseScene
{
private:
	/**
	 * Font used to display debug info
	 */
	//Font m_debugInfoTextFont;

	/**
	 * Text display for debug text
	 */
	//Text m_debugInfoTextDisplay;

	// TODO: Make the crosshair an actual sprite
	/**
	 * Text display for the crosshair
	 */
	//Text m_crossHairTextDisplay;

	/**
	 * Camera
	 */
	Camera m_camera;

	/**
	 * World data
	 */
	World* m_world;

	/**
	 * Chunk indices of the chunk we were previously in
	 */
	glm::ivec3 m_prevChunkIndices;

	/**
	 * Number of chunks to render in each axis that is visible
	 * at a time
	 */
	int m_chunkRenderDistance;

	/**
	 * Sky color
	 */
	glm::vec4 m_skyColor;

public:
	/**
	 * @brief Constructor
	 */
	MainScene();

	/**
	 * @brief Destructor
	 */
	~MainScene();

	/**
     * @brief Initializes the scene.
     */
    void Init() override;

    /**
     * @brief Updates the scene.
     * @param[in] deltaTime Time elapsed since the previous frame
     */
    void Update(const float& deltaTime) override;

    /**
     * @brief Updates the scene with a fixed timestep.
     * @param[in] timestep Fixed timestep
     */
    void FixedUpdate(const float& timestep) override;

    /**
     * @brief Draws the scene.
     */
    void Draw() override;

    /**
     * @brief Cleans up the resources used by the scene.
     */
    void Cleanup() override;
};
