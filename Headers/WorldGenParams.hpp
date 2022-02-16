#pragma once

#include <cstdint>

/**
 * Struct containing parameters related to world generation
 */
struct WorldGenParams
{
    /**
     * World size
     */
    uint32_t worldSize;

    /**
     * World max height
     */
    uint32_t worldMaxHeight;

    /**
     * World seed
     */
    uint32_t seed;

    /**
     * Number of octaves for the noise
     */
    uint32_t noiseNumOctaves;

    /**
     * Noise scale
     */
    float noiseScale;

    /**
     * Noise persistence
     */
    float noisePersistence;

    /**
     * Noise lacunarity
     */
    float noiseLacunarity;
};
