#pragma once

#include "SceneManager.hpp"
#include "Scenes/BaseScene.hpp"

#include <cstdint>

/**
 * Scene for experimenting with various things
 */
class SandboxScene : public BaseScene
{
private:
    
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
    
};
