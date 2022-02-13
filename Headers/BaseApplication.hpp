#pragma once

#include <glad/glad.h>

#include "BaseScene.hpp"
#include "Window.hpp"

class BaseApplication
{
private:
    /**
     * Flag indicating whether the application is running
     */
    bool m_isRunning;

    /**
     * Current scene
     */
    BaseScene* m_activeScene;

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
     * @param[in] initialScene Initial scene
     */
    void Run(BaseScene* initialScene);

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
