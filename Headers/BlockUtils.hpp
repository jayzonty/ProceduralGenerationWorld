#pragma once

#include "Enums/BlockFaceEnum.hpp"

#include <glm/glm.hpp>

#include <array>

/**
 * Class containing utility functions related to blocks
 */
class BlockUtils
{
public:
    /**
     * @brief Gets the offsets for the four corners of the specified block face
     * @param[in] face Face of the block to get the offsets of
     * @return Array containing the 4 offsets
     */
    static std::array<glm::vec3, 4> GetVertexOffsetsFromFace(const BlockFaceEnum &face);

    /**
     * @brief Gets the color tint for the specified block face
     * @param[in] face Face of the block
     * @return Color tint for the specified block face
     */
    static glm::vec4 GetColorTintFromFace(const BlockFaceEnum &face);

    /** 
     * @brief Gets the normal for the specified block face
     * @param[in] face Face of the block
     * @return Normal of the specified block face
     */
    static glm::vec3 GetNormalFromFace(const BlockFaceEnum &face);
};
