#pragma once

#include <glm/glm.hpp>

/**
 * Class containing utility math functions
 */
class MathUtils
{
public:
    /**
     * @brief Performs a linear interpolation between two float scalars
     * @param[in] a First scalar
     * @param[in] b Second scalar
     * @param[in] t Progress
     * @return Interpolated value
     */
    static float Lerp(const float &a, const float &b, const float &t);

    /**
     * @brief Performs a linear interpolation between two vec4's
     * @param[in] a First value
     * @param[in] b Second value
     * @param[in] t Progress
     * @return Interpolated value
     */
    static glm::vec4 Lerp(const glm::vec4 &a, const glm::vec4 &b, const float &t);
};
