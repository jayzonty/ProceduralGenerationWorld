#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace Engine
{
/**
 * Vector struct
 */
template <typename T, uint32_t N>
struct Vector
{
    // Values in the vector
    std::array<T, N> values;

    /**
     * @brief Gets a reference to the x-component
     * @return Reference to the x-component
     */
    T& x()
    {
        static_assert(std::cmp_greater(N, 0), "x() can only be used for vectors with more than 0 dimensions");
        return values[0];
    }

    /**
     * @brief Gets a const reference to the x-component
     * @return Const reference to the X-component
     */
    const T& x() const
    {
        static_assert(std::cmp_greater(N, 0), "x() can only be used for vectors with more than 0 dimensions");
        return values[0];
    }

    /**
     * @brief Gets a reference to the y-component
     * @return Reference to the y-component
     */
    T& y()
    {
        static_assert(std::cmp_greater(N, 1), "y() can only be used for vectors with more than 1 dimensions");
        return values[1];
    }

    /**
     * @brief Gets a const reference to the y-component
     * @return Const reference to the y-component
     */
    const T& y() const
    {
        static_assert(std::cmp_greater(N, 1), "y() can only be used for vectors with more than 1 dimensions");
        return values[1];
    }

    /**
     * @brief Gets a reference to the z-component
     * @return Reference to the z-component
     */
    T& z()
    {
        static_assert(std::cmp_greater(N, 2), "z() can only be used for vectors with more than 2 dimensions");
        return values[2];
    }

    /**
     * @brief Gets a const reference to the z-component
     * @return Const reference to the z-component
     */
    const T& z() const
    {
        static_assert(std::cmp_greater(N, 2), "z() can only be used for vectors with more than 2 dimensions");
        return values[2];
    }

    /**
     * @brief Gets a reference to the component at the specified index
     * @param[in] index
     * @return Reference to the component at the specified index
     */
    inline T& operator[](const size_t& index)
    {
        assert(0 <= index && index < N);
        return values[index];
    }

    /**
     * @brief Gets a const reference to the component at the specified index
     * @param[in] index
     * @return Const reference to the component at the specified index
     */
    inline const T& operator[](const size_t& index) const
    {
        assert(0 <= index && index < N);
        return values[index];
    }
};

/**
 * @brief Operator overload for adding two vectors
 * @param[in] lhs Left-hand side
 * @param[in] rhs Right-hand side
 * @return Sum of the two vectors
 */
template <typename T, uint32_t N>
inline Vector<T, N> operator+(Vector<T, N> lhs, const Vector<T, N>& rhs)
{
    for (uint32_t i = 0; i < N; ++i)
    {
        lhs[i] += rhs[i];
    }
    return lhs;
}

/**
 * @brief Operator overload for subtracting two vectors
 * @param[in] lhs Left-hand side
 * @param[in] rhs Right-hand side
 * @return Difference of the two vectors
 */
template <typename T, uint32_t N>
inline Vector<T, N> operator-(Vector<T, N> lhs, const Vector<T, N>& rhs)
{
    for (uint32_t i = 0; i < N; ++i)
    {
        lhs[i] -= rhs[i];
    }
    return lhs;
}

/**
 * @brief Operator overload for vector-scalar multiplication
 * @param[in] lhs Left-hand side
 * @param[in] rhs Right-hand side
 * @return Result of the vector-scalar multiplication
 */
template <typename T, uint32_t N>
inline Vector<T, N> operator*(Vector<T, N> lhs, const T& rhs)
{
    for (uint32_t i = 0; i < N; ++i)
    {
        lhs[i] *= rhs;
    }
    return lhs;
}

/**
 * @brief Operator overload for vector-scalar division
 * @param[in] lhs Left-hand side
 * @param[in] rhs Right-hand side
 * @return Result of the vector-scalar division
 */
template <typename T, uint32_t N>
inline Vector<T, N> operator/(Vector<T, N> lhs, const T& rhs)
{
    for (uint32_t i = 0; i < N; ++i)
    {
        lhs[i] /= rhs;
    }
    return lhs;
}

/**
 * @brief Gets the square magnitude of the specified vector
 * @param[in] vec Vector to get the square magnitude of
 * @return Square magnitude of the specified vector
 */
template <typename T, uint32_t N>
inline T SquareMagnitude(const Vector<T, N>& vec)
{
    T ret = 0;
    for (uint32_t i = 0; i < N; ++i)
    {
        ret += vec[i] * vec[i];
    }
    return ret;
}

/**
 * @brief Gets the magnitude of the specified vector
 * @param[in] vec Vector to get the magnitude of
 * @return Magnitude of the specified vector
 */
template <typename T, uint32_t N>
inline T Magnitude(const Vector<T, N>& vec)
{
    T sqrMag = SquareMagnitude(vec);
    return std::sqrt(sqrMag);
}

/**
 * @brief Normalizes the specified vector
 * @param[in] vec Vector to normalize
 * @return Normalized version of the specified vector
 */
template <typename T, uint32_t N>
inline Vector<T, N> Normalize(const Vector<T, N>& vec)
{
    Vector<T, N> ret = vec;

    T mag = Magnitude(vec);
    if (mag > 0)
    {
        for (uint32_t i = 0; i < N; ++i)
        {
            ret[i] = vec[i] / mag;
        }
    }

    return ret;
}

/**
 * @brief Returns the 2D-vector perpendicular to the specified 2D-vector
 * @param[in] vec Vector to get the perpendicular vector of
 * @return 2D-vector perpendicular to the specified 2D-vector
 */
template <typename T>
inline Vector<T, 2> Perp(const Vector<T, 2>& vec)
{
    Vector<T, 2> ret;
    ret.x() = -vec.y();
    ret.y() = vec.x();
    return ret;
}

// 2D vector of floats
typedef Vector<float, 2> Vector2f;
// 3D vector of floats
typedef Vector<float, 3> Vector3f;
}
