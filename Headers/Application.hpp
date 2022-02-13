#pragma once

#include "BaseApplication.hpp"

/**
 * Class for the OpenForBusiness application
 */
class Application : public BaseApplication
{
private:

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
