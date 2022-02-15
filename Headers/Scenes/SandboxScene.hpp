#pragma once

#include "FastNoiseLite/FastNoiseLite.h"
#include "Image.hpp"
#include "SceneManager.hpp"
#include "Scenes/BaseScene.hpp"
#include "Texture.hpp"
#include "UIRenderer.hpp"
#include <cstdint>

/**
 * Scene for experimenting with various things
 */
class SandboxScene : public BaseScene
{
private:
    /**
     * Noise engine
     */
    FastNoiseLite m_noise;

    /**
     * UI renderer
     */
    UIRenderer m_uiRenderer;

    /**
     * Noise image
     */
    Image m_noiseImage;

    /**
     * Noise texture
     */
    Texture m_noiseTexture;

    /**
     * Noise seed
     */
    int m_noiseSeed;
    
    /**
     * Number of octaves for the noise
     */
    uint32_t m_noiseNumOctaves;

    /**
     * Noise persistence
     */
    float m_noisePersistence;

    /**
     * Noise lacunarity
     */
    float m_noiseLacunarity;

    /**
     * Noise scale
     */
    float m_noiseScale;
    
public:
    /**
     * @brief Constructor
     * @param[in] sceneManager Scene manager
     */
    SandboxScene(SceneManager *sceneManager);

    /**
     * @brief Destructor
     */
    ~SandboxScene();

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

private:
    /**
     * @brief Updates the image that visualizes the noise map
     */
    void UpdateNoiseImage();
};
