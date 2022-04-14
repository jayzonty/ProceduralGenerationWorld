#ifndef GRAPHICS_PIPELINE_BUILDER_HEADER
#define GRAPHICS_PIPELINE_BUILDER_HEADER

#include <vulkan/vulkan_core.h>

#include <string>
#include <vector>

namespace Vulkan
{
/**
 * Builder class for a Vulkan graphics pipeline
 */
class GraphicsPipelineBuilder
{
private:
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;

    // Create info structs for each pipeline stage
    VkPipelineVertexInputStateCreateInfo m_vertexInputCreateInfo;
    VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyCreateInfo;
    VkPipelineViewportStateCreateInfo m_viewportCreateInfo;
    VkPipelineRasterizationStateCreateInfo m_rasterizationCreateInfo;
    VkPipelineMultisampleStateCreateInfo m_multisampleCreateInfo;
    VkPipelineDepthStencilStateCreateInfo m_depthStencilCreateInfo;
    VkPipelineColorBlendStateCreateInfo m_colorBlendCreateInfo;
    VkPipelineDynamicStateCreateInfo m_dynamicStateCreateInfo;
    VkPipelineLayoutCreateInfo m_pipelineLayoutCreateInfo;
    VkPipelineShaderStageCreateInfo m_vertexShaderCreateInfo;
    VkPipelineShaderStageCreateInfo m_fragmentShaderCreateInfo;

    VkPipelineColorBlendAttachmentState m_colorBlendAttachment;

    std::string m_vertexShaderFilePath;
    std::string m_fragmentShaderFilePath;

    VkRenderPass m_renderPass;

public:
    GraphicsPipelineBuilder();
    ~GraphicsPipelineBuilder();

    // --- Vertex input stage ---
    GraphicsPipelineBuilder& SetVertexBindingDescriptions(const std::vector<VkVertexInputBindingDescription> &descriptions);
    GraphicsPipelineBuilder& SetVertexAttributeDescriptions(const std::vector<VkVertexInputAttributeDescription> &descriptions);

    // --- Input assembly stage ---
    GraphicsPipelineBuilder& SetTopology(const VkPrimitiveTopology &topology);

    // --- Viewport & scissors
    GraphicsPipelineBuilder& SetViewportCount(const uint32_t &viewportCount);
    GraphicsPipelineBuilder& SetViewports(const std::vector<VkViewport> &viewports);
    GraphicsPipelineBuilder& SetScissorCount(const uint32_t &scissorCount);
    GraphicsPipelineBuilder& SetScissors(const std::vector<VkRect2D> &scissors);

    // --- Rasterizer ---
    GraphicsPipelineBuilder& SetPolygonMode(const VkPolygonMode &polygonMode);
    GraphicsPipelineBuilder& SetCullMode(const VkCullModeFlags &cullMode);
    GraphicsPipelineBuilder& SetFrontFace(const VkFrontFace &frontFace);

    // --- Multisample ---

    // --- Depth & stencil ---
    GraphicsPipelineBuilder& SetDepthTestEnabled(const VkBool32 &flag);
    GraphicsPipelineBuilder& SetDepthWriteEnabled(const VkBool32 &flag);
    GraphicsPipelineBuilder& SetDepthCompareOp(const VkCompareOp &compareOp);

    // --- Color blending ---

    // --- Dynamic state ---
    GraphicsPipelineBuilder& SetDynamicStates(const std::vector<VkDynamicState> &dynamicStates);

    // --- Pipeline layout ---
    GraphicsPipelineBuilder& SetDescriptorSetLayouts(const std::vector<VkDescriptorSetLayout> &layouts);

    // --- Shaders ---
    GraphicsPipelineBuilder& SetVertexShaderFilePath(const std::string &vertexShaderFilePath);
    GraphicsPipelineBuilder& SetFragmentShaderFilePath(const std::string &fragmentShaderFilePath);

    // --- Render pass ---
    GraphicsPipelineBuilder& SetRenderPass(const VkRenderPass &renderPass);

    bool Build();

    VkPipelineLayout GetPipelineLayout();
    VkPipeline GetPipeline();

private:
    /**
     * @brief Create a shader module from the provided shader file path.
     * @param[in] shaderFilePath Shader file path
     * @param[in] device Logical device
     * @param[out] outShaderModule Shader module
     * @return Returns true if the shader module creation was successful. Returns false otherwise.
     */
    bool CreateShaderModule(const std::string& shaderFilePath, VkDevice device, VkShaderModule& outShaderModule);
};
}

#endif // GRAPHICS_PIPELINE_BUILDER_HEADER
