#pragma once

namespace Engine
{
/**
 * Base class for a game scene
 */
class BaseScene
{
public:
    /**
     * @brief Constructor
     */
    BaseScene() = default;

    /**
     * @brief Destructor
     */
    virtual ~BaseScene() = default;

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
};
}
