#include "BlockUtils.hpp"
#include "Enums/BlockFaceEnum.hpp"

/**
 * @brief Gets the offsets for the four corners of the specified block face
 * @param[in] face Face of the block to get the offsets of
 * @return Array containing the 4 offsets
 */
std::array<glm::vec3, 4> BlockUtils::GetVertexOffsetsFromFace(const BlockFaceEnum &face)
{
    std::array<glm::vec3, 4> ret;

    if (face == BlockFaceEnum::FRONT)
    {
        ret[0] = {  1.0f,  0.0f,  0.0f };
        ret[1] = {  0.0f,  0.0f,  0.0f };
        ret[2] = {  0.0f,  1.0f,  0.0f };
        ret[3] = {  1.0f,  1.0f,  0.0f };
    }
    else if (face == BlockFaceEnum::BACK)
    {
        ret[0] = {  0.0f,  0.0f,  1.0f };
        ret[1] = {  1.0f,  0.0f,  1.0f };
        ret[2] = {  1.0f,  1.0f,  1.0f };
        ret[3] = {  0.0f,  1.0f,  1.0f };
    }
    else if (face == BlockFaceEnum::LEFT)
    {
        ret[0] = {  1.0f,  0.0f,  1.0f };
        ret[1] = {  1.0f,  0.0f,  0.0f };
        ret[2] = {  1.0f,  1.0f,  0.0f };
        ret[3] = {  1.0f,  1.0f,  1.0f };
    }
    else if (face == BlockFaceEnum::RIGHT)
    {
        ret[0] = {  0.0f,  0.0f,  0.0f };
        ret[1] = {  0.0f,  0.0f,  1.0f };
        ret[2] = {  0.0f,  1.0f,  1.0f };
        ret[3] = {  0.0f,  1.0f,  0.0f };
    }
    else if (face == BlockFaceEnum::TOP)
    {
        ret[0] = {  1.0f,  1.0f,  0.0f };
        ret[1] = {  0.0f,  1.0f,  0.0f };
        ret[2] = {  0.0f,  1.0f,  1.0f };
        ret[3] = {  1.0f,  1.0f,  1.0f };
    }
    else if (face == BlockFaceEnum::BOTTOM)
    {
        ret[0] = {  1.0f,  0.0f,  1.0f };
        ret[1] = {  0.0f,  0.0f,  1.0f };
        ret[2] = {  0.0f,  0.0f,  0.0f };
        ret[3] = {  1.0f,  0.0f,  0.0f };
    }

    return ret;
}

/**
 * @brief Gets the color tint for the specified block face
 * @param[in] face Face of the block
 * @return Color tint for the specified block face
 */
glm::vec4 BlockUtils::GetColorTintFromFace(const BlockFaceEnum &face)
{
    float color = 0.0f;
    if (face == BlockFaceEnum::TOP) color = 0.5f;
    else if (face == BlockFaceEnum::BOTTOM) color = 0.2f;
    else if (face == BlockFaceEnum::LEFT) color = 0.35f;
    else if (face == BlockFaceEnum::RIGHT) color = 0.35f;
    else if (face == BlockFaceEnum::FRONT) color = 0.3f;
    else if (face == BlockFaceEnum::BACK) color = 0.3f;

    return glm::vec4(color, color, color, 1.0f);
}

/** 
 * @brief Gets the normal for the specified block face
 * @param[in] face Face of the block
 * @return Normal of the specified block face
 */
glm::vec3 BlockUtils::GetNormalFromFace(const BlockFaceEnum &face)
{
    if (face == BlockFaceEnum::TOP) return { 0.0f, 1.0f, 0.0f };
    if (face == BlockFaceEnum::BOTTOM) return { 0.0f, -1.0f, 0.0f };
    if (face == BlockFaceEnum::LEFT) return {  1.0f, 0.0f, 0.0f };
    if (face == BlockFaceEnum::RIGHT) return { -1.0f, 0.0f, 0.0f };
    if (face == BlockFaceEnum::FRONT) return { 0.0f, 0.0f, -1.0f };
    if (face == BlockFaceEnum::BACK) return { 0.0f, 0.0f, 1.0f };
    return { 0.0f, 0.0f, 0.0f };
}
