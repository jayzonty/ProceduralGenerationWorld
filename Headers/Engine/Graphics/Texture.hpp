#pragma once

#include <cstdint>

namespace Engine
{
/**
 * Struct containing texture data
 */
struct Texture
{
    /**
     * Texture ID
     */
    uint32_t texID;

    /**
     * Texture width
     */
    int32_t width;

    /**
     * Texture height
     */
    int32_t height;
};
}
