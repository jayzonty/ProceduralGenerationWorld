#pragma once

#include <cstdint>
#include <map>

class BaseScene;

class SceneManager
{
private:
    /**
     * Mapping between an ID and a scene
     */
    std::map<uint32_t, BaseScene*> m_scenes;

    /**
     * Currently active scene
     */
    BaseScene *m_activeScene;

public:
    /**
     * @brief Constructor
     */
    SceneManager();

    /**
     * @brief Destructor
     */
    ~SceneManager();

    /**
     * @brief Gets the currently active scene
     * @return Reference to the currently active scene
     */
    BaseScene* GetActiveScene();

    /**
     * @brief Associates the specified scene ID to the specified scene
     * @param[in] sceneId Scene ID
     * @param[in] scene Reference to the scene
     * @note The scene manager will NOT free the scene upon destruction. As such,
     * the owner of the scene manager should also be the one to free the scene.
     */
    void RegisterScene(const uint32_t& sceneId, BaseScene *scene);

    /**
     * @brief Switches to the scene identified by the specified scene ID
     * @param[in] sceneId Scene ID of the scene to switch to
     */
    void SwitchToScene(const uint32_t& sceneId);
};