#pragma once

#include <glad/glad.h>

#include "Scenes/BaseScene.hpp"
#include "SceneManager.hpp"
#include "Window.hpp"

class BaseApplication
{
private:
    /**
     * Flag indicating whether the application is running
     */
    bool m_isRunning;

    /**
     * Scene manager
     */
    SceneManager m_sceneManager;

public:
    /**
     * @brief Constructor
     */
    BaseApplication();

    /**
     * @brief Destructor
     */
    virtual ~BaseApplication();

    /**
     * @brief Runs the application.
     */
    void Run();

    /**
     * @brief Gets the scene manager for this application
     * @return Reference to the scene manager
     */
    SceneManager* GetSceneManager();

protected:
    /**
     * @brief Overridable function called right after the base application initialization
     */
    virtual void OnInit() {}

    /**
     * @brief Overridable function called right before the base appliction will be cleaned up
     */
    virtual void OnCleanup() {}

private:
    /**
     * @brief Initializes the application.
     * @return Returns true if the initialization was successful.
     * Returns false otherwise.
     */
    bool Init();

    /**
     * @brief Cleans up the application.
     */
    void Cleanup();
};
