#pragma once

#include <glad/glad.h>
#include "Engine/Window.hpp"

#include "Engine/BaseScene.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/ShaderProgram.hpp"

namespace Engine
{
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
}
