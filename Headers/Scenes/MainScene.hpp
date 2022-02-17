#pragma once

#include "BaseScene.hpp"
#include "BatchRenderer.hpp"
#include "Camera.hpp"
#include "Image.hpp"
#include "SceneManager.hpp"
#include "Texture.hpp"
#include "UIRenderer.hpp"
#include "World.hpp"

#include <cstdint>
#include <glm/glm.hpp>

/**
 * Main scene class
 */
class MainScene : public BaseScene
{
private:
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
	 * Current time
	 */
	uint32_t m_currentTime;

	/**
	 * Timer until the next time tick
	 */
	float m_timeTickTimer;

	/**
	 * UI Renderer
	 */
	UIRenderer m_uiRenderer;

	/**
	 * Batch renderer
	 */
	BatchRenderer m_batchRenderer;

	/**
	 * Noise image
	 */
	Image m_noiseImage;

	/**
	 * Texture for visualizing noise map
	 */
	Texture m_noiseTexture;

	/**
	 * Sun mesh
	 */
	Mesh m_sunMesh;

	/**
	 * Moon mesh
	 */
	Mesh m_moonMesh;

public:
	/**
	 * @brief Constructor
	 * @param[in] sceneManager Scene manager
	 */
	MainScene(SceneManager *sceneManager);

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
	void Update(const float &deltaTime) override;

	/**
     * @brief Updates the scene with a fixed timestep.
     * @param[in] timestep Fixed timestep
     */
    void FixedUpdate(const float &timestep) override;

	/**
	 * @brief Draws the scene
	 */
	void Draw() override;

	/**
     * @brief Cleans up the resources used by the scene.
     */
	void Cleanup() override;
};
