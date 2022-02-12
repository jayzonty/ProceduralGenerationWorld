#pragma once

#include <random>

namespace Engine
{
class Random
{
private:
    /**
     * Seed
     */
    int m_seed;

    /**
     * Random engine
     */
    std::mt19937 m_randomEngine;

public:
    /**
     * @brief Constructor
     * @param[in] seed Initial seed
     */
    Random(int seed);

    /**
     * @brief Constructor
     */
    Random();

    /**
     * @brief Generates a random float between 0.0 and 1.0.
     * @return Random float between 0.0 and 1.0
     */
    float RandomFloat();

    /**
     * @brief Generates a random float between the specified minimum and maximum.
     * @param[in] min Minimum value
     * @param[in] max Maximum value
     * @return Random float between the specified minimum and maximum value
     */
    float RandomFloat(const float& min, const float& max);

    /**
     * @brief Generates a random unsigned integer between the specified minimum and maximum.
     * @param[in] min Minimum value
     * @param[in] max Maximum value
     * @return Random unsigned integer between the specified minimum and maximum value
     */
    uint32_t RandomUint32(const uint32_t& min, const uint32_t& max);

    /**
     * @brief Generates a random integer between the specified minimum and maximum.
     * @param[in] min Minimum value
     * @param[in] max Maximum value
     * @return Random integer between the specified minimum and maximum value
     */
    int32_t RandomInt32(const int32_t& min, const int32_t& max);
};
}
