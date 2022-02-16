#include "Utils/MathUtils.hpp"

/**
 * @brief Performs a linear interpolation between two float scalars
 * @param[in] a First scalar
 * @param[in] b Second scalar
 * @param[in] t Progress
 * @return Interpolated value
 */
float MathUtils::Lerp(const float &a, const float &b, const float &t)
{
    return a + t * (b - a);
}

/**
 * @brief Performs a linear interpolation between two vec4's
 * @param[in] a First value
 * @param[in] b Second value
 * @param[in] t Progress
 * @return Interpolated value
 */
glm::vec4 MathUtils::Lerp(const glm::vec4 &a, const glm::vec4 &b, const float &t)
{
    glm::vec4 ret;
    ret.x = Lerp(a.x, b.x, t);
    ret.y = Lerp(a.y, b.y, t);
    ret.z = Lerp(a.z, b.z, t);
    ret.w = Lerp(a.w, b.w, t);
    return ret;
}
