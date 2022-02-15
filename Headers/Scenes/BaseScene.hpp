#pragma once

#include "SceneManager.hpp"

/**
 * Base class for a game scene
 */
class BaseScene
{
private:
    /**
     * Reference to the scene manager
     */
    SceneManager *m_sceneManager;

public:
    /**
     * @brief Constructor
     */
    BaseScene(SceneManager *sceneManager)
        : m_sceneManager(sceneManager)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~BaseScene()
    {
    }

    /**
     * @brief Initializes the scene.
     */
    virtual void Init() = 0;

    /**
     * @brief Updates the scene.
     * @param[in] deltaTime Time elapsed since the previous frame
     */
    virtual void Update(const float& deltaTime) = 0;

    /**
     * @brief Updates the scene with a fixed timestep.
     * @param[in] timestep Fixed timestep
     */
    virtual void FixedUpdate(const float& timestep) = 0;

    /**
     * @brief Draws the scene.
     */
    virtual void Draw() = 0;

    /**
     * @brief Cleans up the resources used by the scene.
     */
    virtual void Cleanup() = 0;

protected:
    /**
     * @brief Gets the scene manager
     * return Scene manager
     */
    SceneManager* GetSceneManager()
    {
        return m_sceneManager;
    }
};
