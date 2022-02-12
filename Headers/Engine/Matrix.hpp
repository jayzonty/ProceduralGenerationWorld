#pragma once

#include <cstring>

namespace Engine
{
/**
 * 4x4 matrix class
 */
struct Matrix4
{
    // Array of values in the matrix (column-major)
    float values[16];

    /**
     * @brief Constructor
     */
    Matrix4()
    {
        memset(values, 0, sizeof(float) * 16);
    }

    /**
     * @brief Copy constructor
     * @param[in] other Matrix to copy
     */
    Matrix4(const Matrix4& other)
    {
        memcpy(values, other.values, sizeof(float) * 16);
    }

    /**
     * @brief Static function for creating an identity matrix.
     * @return 4x4 identity matrix
     */
    static Matrix4 CreateIdentity()
    {
	    Matrix4 ret = {};
	    ret.values[0] = ret.values[5] = ret.values[10] = ret.values[15] = 1.0f;
	    return ret;
    }
};

/**
 * @brief Operator overload for addition of two Matrix4's.
 * @param[in] lhs Left-hand side
 * @param[in] rhs Right-hand side
 * @return Result of the matrix addition
 */
inline Matrix4 operator+(Matrix4 lhs, const Matrix4& rhs)
{
    Matrix4 ret = {};
    for (size_t i = 0; i < 16; ++i)
    {
        ret.values[i] = lhs.values[i] + rhs.values[i];
    }
    return ret;
}

/**
 * @brief Operator overload for multiplication of two Matrix4's.
 * @param[in] lhs Left-hand side
 * @param[in] rhs Right-hand side
 * @return Result of the matrix addition
 */
inline Matrix4 operator*(Matrix4 lhs, const Matrix4& rhs)
{
    Matrix4 ret = {};
    for (size_t column = 0; column < 4; ++column)
    {
        for (size_t row = 0; row < 4; ++row)
        {
            int index = column * 4 + row;
            ret.values[index] =
                lhs.values[0 * 4 + row] * rhs.values[column * 4 + 0] +
                lhs.values[1 * 4 + row] * rhs.values[column * 4 + 1] +
                lhs.values[2 * 4 + row] * rhs.values[column * 4 + 2] +
                lhs.values[3 * 4 + row] * rhs.values[column * 4 + 3];
        }
    }
    return ret;
}
}
