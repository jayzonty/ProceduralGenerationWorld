#pragma once

#include "BaseApplication.hpp"
#include "Scenes/MainScene.hpp"
#include "Scenes/SandboxScene.hpp"

/**
 * Class for the OpenForBusiness application
 */
class Application : public BaseApplication
{
private:
    /**
     * Main scene
     */
    MainScene *m_mainScene;

    /**
     * Sandbox scene
     */
    SandboxScene *m_sandboxScene;

public:
    /**
     * @brief Constructor
     */
    Application();

    /**
     * @brief Destructor
     */
    ~Application();

protected:
    /**
     * @brief Overridable function called right after the base application initialization
     */
    void OnInit() override;

    /**
     * @brief Overridable function called right before the base appliction will be cleaned up
     */
    void OnCleanup() override;
};
