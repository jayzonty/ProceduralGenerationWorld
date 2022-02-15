#include "Utils/NoiseUtils.hpp"

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
float NoiseUtils::GetOctaveNoise(FastNoiseLite &noiseEngine, const float &x, const float &y, const uint32_t &numOctaves, const float &scale, const float &persistence, const float &lacunarity)
{
    float amplitude = 1.0f;
    float frequency = scale;

    float ret = 0.0f;
    float totalAmplitude = 0.0f;
    for (uint32_t i = 0; i < numOctaves; ++i)
    {
        ret += noiseEngine.GetNoise(x * frequency, y * frequency) * amplitude;

        totalAmplitude += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    ret /= totalAmplitude;
    return ret;
}
