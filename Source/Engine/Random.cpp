#include "Engine/Random.hpp"

#include <chrono>
#include <random>

namespace Engine
{
/**
 * @brief Constructor
 * @param[in] seed Initial seed
 */
Random::Random(int seed)
    : m_seed(seed)
    , m_randomEngine(seed)
{
}

/**
 * @brief Constructor
 */
Random::Random()
    : Random(std::chrono::system_clock::now().time_since_epoch().count())
{
}

/**
 * @brief Generates a random float between 0.0 and 1.0.
 * @return Random float between 0.0 and 1.0
 */
float Random::RandomFloat()
{
    std::uniform_real_distribution<float> distribution;
    return distribution(m_randomEngine);
}

/**
 * @brief Generates a random float between the specified minimum and maximum.
 * @param[in] min Minimum value
 * @param[in] max Maximum value
 * @return Random float between the specified minimum and maximum value
 */
float Random::RandomFloat(const float& min, const float& max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(m_randomEngine);
}

/**
 * @brief Generates a random unsigned integer between the specified minimum and maximum.
 * @param[in] min Minimum value
 * @param[in] max Maximum value
 * @return Random unsigned integer between the specified minimum and maximum value
 */
uint32_t Random::RandomUint32(const uint32_t& min, const uint32_t& max)
{
    std::uniform_int_distribution<uint32_t> distribution(min, max);
    return distribution(m_randomEngine);
}

/**
 * @brief Generates a random integer between the specified minimum and maximum.
 * @param[in] min Minimum value
 * @param[in] max Maximum value
 * @return Random integer between the specified minimum and maximum value
 */
int32_t Random::RandomInt32(const int32_t &min, const int32_t &max)
{
    std::uniform_int_distribution<int32_t> distribution(min, max);
    return distribution(m_randomEngine);
}
}
