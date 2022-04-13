#include "BaseApplication.hpp"

#include "Input.hpp"
#include "SceneManager.hpp"
#include "Utils/FileUtils.hpp"
#include "Vertex.hpp"
#include "Vulkan/VulkanContext.hpp"
#include "WindowManager.hpp"

#include <GLFW/glfw3.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>

/**
 * @brief Constructor
 */
BaseApplication::BaseApplication()
    : m_isRunning(false)
    , m_sceneManager()
{
}

/**
 * @brief Destructor
 */
BaseApplication::~BaseApplication()
{
}

/**
 * @brief Runs the application.
 */
void BaseApplication::Run()
{
    // If the application is somehow running when this
    // function is called, don't allow to "re-run".
    if (m_isRunning)
    {
        return;
    }
    m_isRunning = true;

    if (!Init())
    {
        std::cout << "[Application] Failed to initialize application!" << std::endl;
        return;
    }
    OnInit();

    std::array<Vertex, 3> vertices;
    vertices[0].position = { -0.5f,  0.5f,  0.0f };
    vertices[0].color = { 1.0f, 0.0f, 0.0f };
    vertices[1].position = {  0.5f,  0.5f,  0.0f };
    vertices[1].color = { 0.0f, 1.0f, 0.0f };
    vertices[2].position = {  0.0f,  -0.5f,  0.0f };
    vertices[2].color = { 0.0f, 0.0f, 1.0f };

    if (!m_testVertexBuffer.Create(sizeof(Vertex) * 3, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
    {
        std::cerr << "Failed to create vertex buffer!" << std::endl;
    }
    void *data = m_testVertexBuffer.MapMemory(0, sizeof(Vertex) * 3);
    memcpy(data, vertices.data(), sizeof(Vertex) * 3);
    m_testVertexBuffer.UnmapMemory();

    double prevTime = glfwGetTime();

    Window* mainWindow = WindowManager::GetMainWindow();

    uint32_t currentFrame = 0;

    // Game loop
    while (!mainWindow->IsClosed())
    {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - prevTime);
        prevTime = currentTime;

        m_sceneManager.GetActiveScene()->Update(deltaTime);
        m_sceneManager.GetActiveScene()->FixedUpdate(0.0f);

        // --- Draw frame start ---

        // Wait for the current frame to be done rendering
        vkWaitForFences
        (
            VulkanContext::GetLogicalDevice(),
            1,
            &m_frameDataList[currentFrame].renderDoneFence,
            VK_TRUE,
            UINT64_MAX
        );
        vkResetFences
        (
            VulkanContext::GetLogicalDevice(), 
            1, 
            &m_frameDataList[currentFrame].renderDoneFence
        );

        uint32_t nextImageIndex;
        VkResult acquireImageResult = vkAcquireNextImageKHR
        (
            VulkanContext::GetLogicalDevice(), 
            m_vkSwapchain, 
            UINT64_MAX, 
            m_frameDataList[currentFrame].imageAvailableSemaphore, 
            VK_NULL_HANDLE, 
            &nextImageIndex
        );
        if ((acquireImageResult != VK_SUCCESS) && (acquireImageResult != VK_SUBOPTIMAL_KHR))
        {
            std::cerr << "Failed to acquire next image!" << std::endl;
            glfwSetWindowShouldClose(WindowManager::GetMainWindow()->GetWindowHandle(), GLFW_TRUE);
            continue;
        }

        // --- Build render commands ---
        
        // Start command buffer recording
        VkCommandBuffer &commandBuffer = m_frameDataList[currentFrame].commandBuffer;
        vkResetCommandBuffer(commandBuffer, 0);
        VkCommandBufferBeginInfo commandBufferBeginInfo {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo) != VK_SUCCESS)
        {
            std::cout << "Failed to begin recording command buffer!" << std::endl;
            glfwSetWindowShouldClose(WindowManager::GetMainWindow()->GetWindowHandle(), GLFW_TRUE);
            continue;
        }

        // Begin render pass
        VkRenderPassBeginInfo renderPassBeginInfo {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = m_vkRenderPass;
        renderPassBeginInfo.framebuffer = m_frameDataList[currentFrame].framebuffer;
        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = m_vkSwapchainImageExtent;
        std::array<VkClearValue, 1> clearValues;
        clearValues[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = clearValues.data();
        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Viewport and scissors are dynamic, so we set here as a command
        VkViewport viewport {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_vkSwapchainImageExtent.width);
        viewport.height = static_cast<float>(m_vkSwapchainImageExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor {};
        scissor.offset = { 0, 0 };
        scissor.extent = m_vkSwapchainImageExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkPipeline);

        VkDeviceSize offset = 0;
        VkBuffer vertexBuffers[] = { m_testVertexBuffer.GetHandle() };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, &offset);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            std::cerr << "Failed to end recording of command buffer!" << std::endl;
            glfwSetWindowShouldClose(WindowManager::GetMainWindow()->GetWindowHandle(), GLFW_TRUE);
            continue;
        }

        // --- Submit ---
        VkSubmitInfo submitInfo {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_frameDataList[currentFrame].imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = { m_frameDataList[currentFrame].renderDoneSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;


        VkResult submitResult = vkQueueSubmit
        (
            VulkanContext::GetGraphicsQueue(),
            1,
            &submitInfo,
            m_frameDataList[currentFrame].renderDoneFence
        );
        if (submitResult != VK_SUCCESS)
        {
            std::cerr << "Failed to submit!" << std::endl;
            glfwSetWindowShouldClose(WindowManager::GetMainWindow()->GetWindowHandle(), GLFW_TRUE);
            continue;
        }

        // --- Present ---
        VkPresentInfoKHR presentInfo {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapchains[] = { m_vkSwapchain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = &nextImageIndex;

        currentFrame = (currentFrame + 1) % m_maxFramesInFlight;

        VkResult presentResult = vkQueuePresentKHR
        (
            VulkanContext::GetPresentQueue(),
            &presentInfo
        );
        if (presentResult != VK_SUCCESS)
        {
            std::cerr << "Failed to present!" << std::endl;
            glfwSetWindowShouldClose(WindowManager::GetMainWindow()->GetWindowHandle(), GLFW_TRUE);
            continue;
        }

        // --- Draw frame end ---

        //m_sceneManager.GetActiveScene()->Draw();

        Input::Prepare();
        mainWindow->PollEvents();
    }

    vkDeviceWaitIdle(VulkanContext::GetLogicalDevice());

    m_sceneManager.GetActiveScene()->Cleanup();
    OnCleanup();
    Cleanup();
}

/**
 * @brief Gets the scene manager for this application
 * @return Reference to the scene manager
 */
SceneManager* BaseApplication::GetSceneManager()
{
    return &m_sceneManager;
}

/**
 * @brief Initializes the application.
 * @return Returns true if the initialization was successful.
 * Returns false otherwise.
 */
bool BaseApplication::Init()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cout << "[Application] Failed to initialize GLFW!" << std::endl;
        return false;
    }

    Window* mainWindow = WindowManager::GetMainWindow();
    if (!mainWindow->Init(800, 600, "Application"))
    {
        std::cout << "[Application] Failed to create GLFW window!" << std::endl;
        return false;
    }

    // --- Register callbacks ---
    GLFWwindow* windowHandle = mainWindow->GetWindowHandle();

    if (!VulkanContext::Initialize(windowHandle))
    {
        std::cerr << "[BaseApplication] Failed to initialize Vulkan context!" << std::endl;
    }
    if (!InitSwapchain())
    {
        std::cerr << "[BaseApplication] Failed to initialize Vulkan swapchain!" << std::endl;
    }
    if (!InitRenderPass())
    {
        std::cerr << "[BaseApplication] Failed to initialize Vulkan renderpass!" << std::endl;
    }
    if (!InitFramebuffers())
    {
        std::cerr << "[BaseApplication] Failed to initialize Vulkan framebuffers!" << std::endl;
    }
    if (!InitCommandPool())
    {
        std::cerr << "[BaseApplication] Failed to initialize Vulkan command pool" << std::endl;
    }
    if (!InitCommandBuffers())
    {
        std::cerr << "[BaseApplication] Failed to initialize Vulkan command buffers!" << std::endl;
    }
    if (!InitGraphicsPipeline())
    {
        std::cerr << "[BaseApplication] Failed to initialize graphics pipeline!" << std::endl;
    }
    if (!InitSynchronizationTools())
    {
        std::cerr << "[BaseApplication] Failed to initialize synchronization tools!" << std::endl;
    }

    // Set the callback function for when the framebuffer size changed
	//glfwSetFramebufferSizeCallback(windowHandle, Application::FramebufferSizeChangedCallback);

    // Disable the cursor.
    // TODO: Maybe have a function inside the window class for this
    glfwSetInputMode(mainWindow->GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the callback function for when a key was pressed
	glfwSetKeyCallback(windowHandle, Input::KeyCallback);

	// Register callback function for when a mouse button was pressed
	glfwSetMouseButtonCallback(windowHandle, Input::MouseButtonCallback);

    // Register callback function for when the mouse scroll wheel was scrolled
    glfwSetScrollCallback(windowHandle, Input::MouseScrollCallback);

	// Register callback function for when the mouse cursor's position changed
	glfwSetCursorPosCallback(windowHandle, Input::CursorCallback);

	// Register callback function for when the mouse cursor entered/left the window
	glfwSetCursorEnterCallback(windowHandle, Input::CursorEnterCallback);

    return true;
}

/**
 * @brief Cleans up the application.
 */
void BaseApplication::Cleanup()
{
    m_testVertexBuffer.Cleanup();

    for (size_t i = 0; i < m_frameDataList.size(); i++)
    {
        vkDestroyFence(VulkanContext::GetLogicalDevice(), m_frameDataList[i].renderDoneFence, nullptr);
        vkDestroySemaphore(VulkanContext::GetLogicalDevice(), m_frameDataList[i].imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(VulkanContext::GetLogicalDevice(), m_frameDataList[i].renderDoneSemaphore, nullptr);
    }

    vkDestroyPipeline(VulkanContext::GetLogicalDevice(), m_vkPipeline, nullptr);
    m_vkPipeline = VK_NULL_HANDLE;
    vkDestroyPipelineLayout(VulkanContext::GetLogicalDevice(), m_vkPipelineLayout, nullptr);
    m_vkPipelineLayout = VK_NULL_HANDLE;

    for (size_t i = 0; i < m_frameDataList.size(); i++)
    {
        vkDestroyFramebuffer(VulkanContext::GetLogicalDevice(), m_frameDataList[i].framebuffer, nullptr);
        m_frameDataList[i].imageView.Cleanup();
        vkFreeCommandBuffers(VulkanContext::GetLogicalDevice(), m_vkCommandPool, 1, &m_frameDataList[i].commandBuffer);
    }
    m_frameDataList.clear();

    vkDestroyCommandPool(VulkanContext::GetLogicalDevice(), m_vkCommandPool, nullptr);
    m_vkCommandPool = VK_NULL_HANDLE;

    vkDestroyRenderPass(VulkanContext::GetLogicalDevice(), m_vkRenderPass, nullptr);
    m_vkRenderPass = VK_NULL_HANDLE;

    vkDestroySwapchainKHR(VulkanContext::GetLogicalDevice(), m_vkSwapchain, nullptr);
    m_vkSwapchain = VK_NULL_HANDLE;

    VulkanContext::Cleanup();

    Window* mainWindow = WindowManager::GetMainWindow();
    mainWindow->Cleanup();
    glfwTerminate();
}

/**
 * @brief Initializes the Vulkan render pass.
 * @return Returns true if the initialization was successful. Returns false otherwise.
 */
bool BaseApplication::InitSwapchain()
{
    // --- Preparations for swapchain creation ---

    // Query surface capabilities (can also do this as a criteria when selecting a physical device)
    // For now, we'll just do a delayed check.
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanContext::GetPhysicalDevice(), VulkanContext::GetVulkanSurface(), &surfaceCapabilities);

    // Query supported formats
    uint32_t numAvailableSurfaceFormats;
    vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanContext::GetPhysicalDevice(), VulkanContext::GetVulkanSurface(), &numAvailableSurfaceFormats, nullptr);
    if (numAvailableSurfaceFormats == 0)
    {
        std::cout << "Selected physical device has no supported surface formats!" << std::endl;
        return false;
    }
    std::vector<VkSurfaceFormatKHR> availableSurfaceFormats(numAvailableSurfaceFormats);
    vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanContext::GetPhysicalDevice(), VulkanContext::GetVulkanSurface(), &numAvailableSurfaceFormats, availableSurfaceFormats.data());

    // Query presentation modes
    uint32_t numAvailablePresentModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanContext::GetPhysicalDevice(), VulkanContext::GetVulkanSurface(), &numAvailablePresentModes, nullptr);
    if (numAvailablePresentModes == 0)
    {
        std::cout << "Selected physical device has no supported present modes!" << std::endl;
        return false;
    }
    std::vector<VkPresentModeKHR> availablePresentModes(numAvailablePresentModes);
    vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanContext::GetPhysicalDevice(), VulkanContext::GetVulkanSurface(), &numAvailablePresentModes, availablePresentModes.data());

    // Select the preferred surface format.
    // Ideally, a format that supports BGRA with SRGB colorspace, but if we can't find such a format, just use the first one
    VkSurfaceFormatKHR selectedSurfaceFormat = availableSurfaceFormats[0];
    for (size_t i = 1; i < availableSurfaceFormats.size(); ++i)
    {
        if ((availableSurfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) && (availableSurfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
        {
            selectedSurfaceFormat = availableSurfaceFormats[i];
            break;
        }
    }

    // Select preferred present mode.
    // By default, GPUs should support VK_PRESENT_MODE_FIFO_KHR at the bare minimum, but
    // if we can find VK_PRESENT_MODE_MAILBOX_KHR, then we use that
    VkPresentModeKHR selectedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (size_t i = 0; i < availablePresentModes.size(); ++i)
    {
        if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            selectedPresentMode = availablePresentModes[i];
            break;
        }
    }

    // Calculate swapchain extent
    VkExtent2D swapchainImageExtent;
    if (surfaceCapabilities.currentExtent.width != UINT32_MAX)
    {
        // Use the extent provided to us by Vulkan
        swapchainImageExtent = surfaceCapabilities.currentExtent;
    }
    else
    {
        // Use the framebuffer size provided by GLFW as the extent.
        // However, we make sure that the extent is within the capabilities of the surface
        int width, height;
        glfwGetFramebufferSize(WindowManager::GetMainWindow()->GetWindowHandle(), &width, &height);

        swapchainImageExtent.width = std::clamp(static_cast<uint32_t>(width), surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
        swapchainImageExtent.height = std::clamp(static_cast<uint32_t>(height), surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
    }

    // --- Create swapchain ---

    // Prepare swapchain create struct
    uint32_t numSwapchainImages = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0)
    {
        numSwapchainImages = std::min(numSwapchainImages, surfaceCapabilities.maxImageCount);
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = VulkanContext::GetVulkanSurface();
    swapchainCreateInfo.minImageCount = numSwapchainImages;
    swapchainCreateInfo.imageFormat = selectedSurfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = selectedSurfaceFormat.colorSpace;
    swapchainCreateInfo.presentMode = selectedPresentMode;
    swapchainCreateInfo.clipped = VK_TRUE; // Clip pixels that are obscured, e.g., by other windows
    swapchainCreateInfo.imageExtent = swapchainImageExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Can use VK_IMAGE_USAGE_TRANSFER_DST_BIT for rendering to an offscreen buffer (e.g., post-processing)
    if (VulkanContext::GetGraphicsQueueIndex() != VulkanContext::GetPresentQueueIndex())
    {
        // Graphics queue and present queue are different (in some GPUs, they can be the same).
        // In this case, we use VK_SHARING_MODE_CONCURRENT, which means swapchain images can be owned by
        // multiple queue famillies without the need for transfer of ownership.
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;

        // We also need to specify which queue families concurrently own a swapchain image 
        uint32_t indices[] = { VulkanContext::GetGraphicsQueueIndex(), VulkanContext::GetPresentQueueIndex() };
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = indices;
    }
    else
    {
        // If graphics queue and present queue are the same, set to exclusive mode since
        // we don't need to transfer ownership normally. This also offers the best performance.
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    // In case we want to apply a transformation to be applied to all swapchain images.
    // If not, we just use the current transformation of the surface capabilities struct.
    swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    // Blending with other windows. (We almost always ignore)
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    // In case the previous swap chain becomes invalid. In that case, we need to supply the old swapchain
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    // Actually create the swapchain
    if (vkCreateSwapchainKHR(VulkanContext::GetLogicalDevice(), &swapchainCreateInfo, nullptr, &m_vkSwapchain) != VK_SUCCESS)
    {
        std::cout << "Failed to create swapchain!" << std::endl;
        return false;
    }

    // Retrieve swapchain images, and store the image format and extent
    vkGetSwapchainImagesKHR(VulkanContext::GetLogicalDevice(), m_vkSwapchain, &numSwapchainImages, nullptr);
    std::vector<VkImage> swapchainImages;
    swapchainImages.resize(numSwapchainImages);
    vkGetSwapchainImagesKHR(VulkanContext::GetLogicalDevice(), m_vkSwapchain, &numSwapchainImages, swapchainImages.data());
    m_vkSwapchainImageFormat = selectedSurfaceFormat.format;
    m_vkSwapchainImageExtent = swapchainImageExtent;

    for (size_t i = 0; i < swapchainImages.size(); ++i)
    {
        m_frameDataList.emplace_back();

        m_frameDataList.back().image = swapchainImages[i];
        if (!m_frameDataList.back().imageView.Create(swapchainImages[i], m_vkSwapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT))
        {
            std::cout << "Failed to create swapchain image views!" << std::endl;
            return false;
        }
    }

    m_maxFramesInFlight = static_cast<uint32_t>(swapchainImages.size());

    return true;
}

/**
 * @brief Initializes the Vulkan render pass.
 * @return Returns true if the initialization was successful. Returns false otherwise.
 */
bool BaseApplication::InitRenderPass()
{
    // TODO: Research more about subpass dependencies

    // Setup color attachment for the framebuffer
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = m_vkSwapchainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Clear framebuffer upon being loaded
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Keep rendered contents in memory after
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // We don't use stencil testing, so we don't care for now
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // We don't use stencil testing, so we don't care for now
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentReference = {};
    colorAttachmentReference.attachment = 0; // This is reflected in the "layout(location = 0) out color" in the fragment shader
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Setup depth attachment for the framebuffer
    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = VK_FORMAT_D32_SFLOAT; // TODO: Create a routine for finding the suitable depth format
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentReference = {};
    depthAttachmentReference.attachment = 1;
    depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // Rendering subpass (not compute subpass)
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pDepthStencilAttachment = nullptr;//&depthAttachmentReference;

    // TODO: Research more about subpass dependencies to understand what the following lines do
    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;// | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;// | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;// | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 1> attachments = { colorAttachment };//, depthAttachment};
    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassCreateInfo.pAttachments = attachments.data();
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &subpassDependency;

    if (vkCreateRenderPass(VulkanContext::GetLogicalDevice(), &renderPassCreateInfo, nullptr, &m_vkRenderPass) != VK_SUCCESS)
    {
        std::cout << "Failed to create render pass!" << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Initializes the Vulkan framebuffers.
 * @return Returns true if the initialization was successful. Returns false otherwise.
 */
bool BaseApplication::InitFramebuffers()
{
    for (size_t i = 0; i < m_frameDataList.size(); ++i)
    {
        std::array<VkImageView, 1> attachments =
        {
            m_frameDataList[i].imageView.GetHandle(),
            //m_vkDepthBufferImageView.GetHandle()
        };

        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = m_vkRenderPass;
        framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferCreateInfo.pAttachments = attachments.data();
        framebufferCreateInfo.width = m_vkSwapchainImageExtent.width;
        framebufferCreateInfo.height = m_vkSwapchainImageExtent.height;
        framebufferCreateInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanContext::GetLogicalDevice(), &framebufferCreateInfo, nullptr, &m_frameDataList[i].framebuffer) != VK_SUCCESS)
        {
            std::cout << " FAILED!" << std::endl;
            return false;
        }
    }
    return true;
}

/**
 * @brief Initializes the Vulkan command pool.
 * @return Returns true if the initialization was successful. Returns false otherwise.
 */
bool BaseApplication::InitCommandPool()
{
    VkCommandPoolCreateInfo commandPoolInfo = {};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.queueFamilyIndex = VulkanContext::GetGraphicsQueueIndex();
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(VulkanContext::GetLogicalDevice(), &commandPoolInfo, nullptr, &m_vkCommandPool) != VK_SUCCESS)
    {
        std::cout << "Failed to create command pool!" << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Initializes the Vulkan command buffer.
 * @return Returns true if the initialization was successful. Returns false otherwise.
 */
bool BaseApplication::InitCommandBuffers()
{
    VkCommandBufferAllocateInfo commandBufferInfo = {};
    commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferInfo.commandPool = m_vkCommandPool;
    commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferInfo.commandBufferCount = m_maxFramesInFlight;

    std::vector<VkCommandBuffer> commandBuffers;
    commandBuffers.resize(m_maxFramesInFlight);
    if (vkAllocateCommandBuffers(VulkanContext::GetLogicalDevice(), &commandBufferInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        std::cout << "Failed to create command buffers!" << std::endl;
        return false;
    }

    for (size_t i = 0; i < m_frameDataList.size(); i++)
    {
        m_frameDataList[i].commandBuffer = commandBuffers[i];
    }

    return true;
}

/**
 * @brief Create Vulkan graphics pipeline
 * @return Returns true if the creation was successful. Returns false otherwise.
 */
bool BaseApplication::InitGraphicsPipeline()
{
    // Set up the fixed pipeline stages

    // Vertex input
    VkVertexInputBindingDescription vertexInputBindingDescription = Vertex::GetBindingDescription();
    std::array<VkVertexInputAttributeDescription, 3> vertexInputAttributeDescriptions = Vertex::GetAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
    vertexInputCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
    vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributeDescriptions.size());
    vertexInputCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data();

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
    inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

    // Viewport and scissors
    VkPipelineViewportStateCreateInfo viewportCreateInfo = {};
    viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportCreateInfo.viewportCount = 1;
    viewportCreateInfo.pViewports = nullptr; // Meant to be dynamic, so this will be ignored
    viewportCreateInfo.scissorCount = 1;
    viewportCreateInfo.pScissors = nullptr;  // Meant to be dynamic, so this will be ignored

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo = {};
    rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationCreateInfo.depthClampEnable = VK_FALSE;
    rasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationCreateInfo.lineWidth = 1.0f;
    rasterizationCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationCreateInfo.depthBiasEnable = VK_FALSE;
    rasterizationCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
    rasterizationCreateInfo.depthBiasClamp = 0.0f; // Optional
    rasterizationCreateInfo.depthBiasSlopeFactor = 0.0f; // Optional

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampleCreateInfo = {};
    multisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleCreateInfo.minSampleShading = 1.0f; // Optional
    multisampleCreateInfo.pSampleMask = nullptr; // Optional
    multisampleCreateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampleCreateInfo.alphaToOneEnable = VK_FALSE; // Optional

    // Depth and stencil testing
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
    depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilInfo.depthTestEnable = VK_FALSE; // Enable depth testing
    depthStencilInfo.depthWriteEnable = VK_FALSE; // New depth of fragments that pass the depth test should be written
    depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS; // Fragments with lesser depth pass the depth test
    // For depth bound test
    depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilInfo.minDepthBounds = 0.0f;
    depthStencilInfo.maxDepthBounds = 1.0f;
    // For stencil testing
    depthStencilInfo.stencilTestEnable = VK_FALSE; 
    depthStencilInfo.front = {};
    depthStencilInfo.back = {};

    // Color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = {};
    colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendCreateInfo.logicOpEnable = VK_FALSE;
    colorBlendCreateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlendCreateInfo.attachmentCount = 1;
    colorBlendCreateInfo.pAttachments = &colorBlendAttachment;
    colorBlendCreateInfo.blendConstants[0] = 0.0f; // Optional
    colorBlendCreateInfo.blendConstants[1] = 0.0f; // Optional
    colorBlendCreateInfo.blendConstants[2] = 0.0f; // Optional
    colorBlendCreateInfo.blendConstants[3] = 0.0f; // Optional

    // Dynamic state (Attributes specified here will have to be specified at drawing time)
    VkDynamicState dynamicStates[] =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
    dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount = 2;
    dynamicStateCreateInfo.pDynamicStates = dynamicStates;

    // Create pipeline layout
    //std::array<VkDescriptorSetLayout, 4> descriptorSetLayouts = { m_vkPerFrameDescriptorSetLayout, m_vkPerObjectDescriptorSetLayout, m_vkSingleTextureDescriptorSetLayout, m_vkSingleTextureDescriptorSetLayout };
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 0;//static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutCreateInfo.pSetLayouts = nullptr;//descriptorSetLayouts.data();
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(VulkanContext::GetLogicalDevice(), &pipelineLayoutCreateInfo, nullptr, &m_vkPipelineLayout) != VK_SUCCESS)
    {
        return false;
    }

    // Create shader modules for the vertex and fragment shaders
    VkShaderModule vertexShaderModule;
    if (!CreateShaderModule("Resources/Shaders/basic_vert.spv", VulkanContext::GetLogicalDevice(), vertexShaderModule))
    {
        return false;
    }

    // Create pipeline stage for the vertex shader using the vertex shader module
    VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = {};
    vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageCreateInfo.module = vertexShaderModule;
    vertexShaderStageCreateInfo.pName = "main";

    VkShaderModule fragmentShaderModule;
    if (!CreateShaderModule("Resources/Shaders/basic_frag.spv", VulkanContext::GetLogicalDevice(), fragmentShaderModule))
    {
        vkDestroyShaderModule(VulkanContext::GetLogicalDevice(), vertexShaderModule, nullptr);
        return false;
    }

    // Create pipeline stage for the fragment shader using the fragment shader module
    VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo = {};
    fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageCreateInfo.module = fragmentShaderModule;
    fragmentShaderStageCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo };

    // Create graphics pipeline
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
    pipelineCreateInfo.pViewportState = &viewportCreateInfo;
    pipelineCreateInfo.pRasterizationState = &rasterizationCreateInfo;
    pipelineCreateInfo.pMultisampleState = &multisampleCreateInfo;
    pipelineCreateInfo.pDepthStencilState = &depthStencilInfo;
    pipelineCreateInfo.pColorBlendState = &colorBlendCreateInfo;
    pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
    pipelineCreateInfo.layout = m_vkPipelineLayout;
    pipelineCreateInfo.renderPass = m_vkRenderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // In case we inherit from an old pipeline
    pipelineCreateInfo.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(VulkanContext::GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_vkPipeline) != VK_SUCCESS)
    {
        vkDestroyShaderModule(VulkanContext::GetLogicalDevice(), vertexShaderModule, nullptr);
        vkDestroyShaderModule(VulkanContext::GetLogicalDevice(), fragmentShaderModule, nullptr);
        return false;
    }

    // Make sure to destroy the shader modules after the pipeline has been created
    vkDestroyShaderModule(VulkanContext::GetLogicalDevice(), vertexShaderModule, nullptr);
    vertexShaderModule = VK_NULL_HANDLE;
    vkDestroyShaderModule(VulkanContext::GetLogicalDevice(), fragmentShaderModule, nullptr);
    fragmentShaderModule = VK_NULL_HANDLE;

    return true;
}

/**
 * @brief Create a shader module from the provided shader file path.
 * @param[in] shaderFilePath Shader file path
 * @param[in] device Logical device
 * @param[out] outShaderModule Shader module
 * @return Returns true if the shader module creation was successful. Returns false otherwise.
 */
bool BaseApplication::CreateShaderModule(const std::string& shaderFilePath, VkDevice device, VkShaderModule& outShaderModule)
{
    std::vector<char> shaderData;
    if (!FileUtils::ReadFileAsBinary(shaderFilePath, shaderData))
    {
        return false;
    }

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = static_cast<uint32_t>(shaderData.size());
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderData.data());

    if (vkCreateShaderModule(device, &createInfo, nullptr, &outShaderModule) != VK_SUCCESS)
    {
        return false;
    }

    return true;
}

/**
 * @brief Initializes the Vulkan synchronization tools.
 * @return Returns true if the initialization was successful. Returns false otherwise.
 */
bool BaseApplication::InitSynchronizationTools()
{
    // --- Create semaphores that we use for rendering ---
    for (uint32_t i = 0; i < m_maxFramesInFlight; ++i)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Set the flag to be already signalled at the start

        if ((vkCreateSemaphore(VulkanContext::GetLogicalDevice(), &semaphoreCreateInfo, nullptr, &m_frameDataList[i].imageAvailableSemaphore) != VK_SUCCESS) 
                || (vkCreateSemaphore(VulkanContext::GetLogicalDevice(), &semaphoreCreateInfo, nullptr, &m_frameDataList[i].renderDoneSemaphore) != VK_SUCCESS)
                || (vkCreateFence(VulkanContext::GetLogicalDevice(), &fenceCreateInfo, nullptr, &m_frameDataList[i].renderDoneFence) != VK_SUCCESS))
        {
            std::cout << "Failed to create synchronization tools!" << std::endl;
            return false;
        }
    }

    return true;
}
