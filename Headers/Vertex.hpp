#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.hpp>

#include <array>

/**
 * Struct containing data about a vertex
 */
struct Vertex
{
    /**
	 * Position
     */
    glm::vec3 position;

    /**
	 * Color
     */
    glm::vec3 color;

    /**
	 * Texture coordinates
     */
    glm::vec2 uv;

    /**
	 * Normal
     */
    glm::vec3 normal;

    static VkVertexInputBindingDescription GetBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription;
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

        // Position
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].offset = offsetof(Vertex, position);
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // Three 32-bit signed floats

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT; // Three 32-bit signed floats

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].offset = offsetof(Vertex, uv);
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT; // Two 32-bit signed floats

        return attributeDescriptions;
    }
};
