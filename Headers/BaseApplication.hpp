#pragma once

#include "Scenes/BaseScene.hpp"
#include "SceneManager.hpp"
#include "Vertex.hpp"
#include "Window.hpp"

#include "Vulkan/VulkanBuffer.hpp"
#include "Vulkan/VulkanImageView.hpp"

#include <vulkan/vulkan.hpp>

class BaseApplication
{
private:
    /**
     * Struct containing data needed for a frame
     */
    struct FrameData
    {
        /**
         * Frame image
         */
        VkImage image;

        /**
         * Frame image view
         */
        VulkanImageView imageView;

        /**
         * Framebuffer
         */
        VkFramebuffer framebuffer;

        /**
         * Frame commandbuffer
         */
        VkCommandBuffer commandBuffer;

        // --- Synchronization ---

        /**
         * Semaphore that is signalled when the next image to use
         * is available and ready for rendering
         */
        VkSemaphore imageAvailableSemaphore;

        /**
         * Semaphore that is signalled when the rendering has been done
         * and is render for presenting (used for waiting before presenting)
         */
        VkSemaphore renderDoneSemaphore;

        /**
         * Fence signalled when the rendering for this frame is done
         */
        VkFence renderDoneFence;
    };

private:
    /**
     * Flag indicating whether the application is running
     */
    bool m_isRunning;

    /**
     * Scene manager
     */
    SceneManager m_sceneManager;

    /**
     * Swapchain
     */
    VkSwapchainKHR m_vkSwapchain;

    /**
     * Swapchain image format
     */
    VkFormat m_vkSwapchainImageFormat;

    /**
     * Swapchain image extent
     */
    VkExtent2D m_vkSwapchainImageExtent;

    /**
     * Maximum number of frames in flight
     */
    uint32_t m_maxFramesInFlight;

    /**
     * Render pass
     */
    VkRenderPass m_vkRenderPass;

    /**
     * Pipeline layout
     */
    VkPipelineLayout m_vkPipelineLayout;

    /**
     * Pipeline
     */
    VkPipeline m_vkPipeline;

    /**
     * Command pool
     */
    VkCommandPool m_vkCommandPool;

    /**
     * List containing data for each frame
     */
    std::vector<FrameData> m_frameDataList;

    /**
     * Test vertex buffer
     */
    VulkanBuffer m_testVertexBuffer;

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

    /**
     * @brief Initializes the Vulkan render pass.
     * @return Returns true if the initialization was successful. Returns false otherwise.
     */
    bool InitSwapchain();

    /**
     * @brief Initializes the Vulkan render pass.
     * @return Returns true if the initialization was successful. Returns false otherwise.
     */
    bool InitRenderPass();

    /**
     * @brief Initializes the Vulkan framebuffers.
     * @return Returns true if the initialization was successful. Returns false otherwise.
     */
    bool InitFramebuffers();

    /**
     * @brief Initializes the Vulkan command pool.
     * @return Returns true if the initialization was successful. Returns false otherwise.
     */
    bool InitCommandPool();

    /**
     * @brief Initializes the Vulkan command buffer.
     * @return Returns true if the initialization was successful. Returns false otherwise.
     */
    bool InitCommandBuffers();

    /**
     * @brief Create Vulkan graphics pipeline
     * @return Returns true if the creation was successful. Returns false otherwise.
     */
    bool InitGraphicsPipeline();

    /**
     * @brief Create a shader module from the provided shader file path.
     * @param[in] shaderFilePath Shader file path
     * @param[in] device Logical device
     * @param[out] outShaderModule Shader module
     * @return Returns true if the shader module creation was successful. Returns false otherwise.
     */
    bool CreateShaderModule(const std::string& shaderFilePath, VkDevice device, VkShaderModule& outShaderModule);

    /**
     * @brief Initializes the Vulkan synchronization tools.
     * @return Returns true if the initialization was successful. Returns false otherwise.
     */
    bool InitSynchronizationTools();
};
