#pragma once

#include "FastNoiseLite/FastNoiseLite.h"
#include <cstdint>

/**
 * Class containing utility functions related to noise generation
 */
class NoiseUtils
{
public:
    /**
     * @brief Gets the noise value with octaves applied
     * @param[in] noiseEngine Noise engine that generates the noise values
     * @param[in] x X-coordinate
     * @param[in] y Y-coordinate
     * @param[in] numOctave Number of octaves
     * @param[in] scale Scale
     * @param[in] persistence Persistence
     * @param[in] lacunarity Lacunarity
     */
    static float GetOctaveNoise(FastNoiseLite &noiseEngine, const float &x, const float &y, const uint32_t &numOctaves, const float &scale, const float &persistence, const float &lacunarity);
};
