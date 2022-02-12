#pragma once

#include "Engine/Matrix.hpp"

namespace Engine::MatrixTransform
{
/**
 * @brief Creates a translation matrix
 * @param[in] tx Translation amount in the x-axis
 * @param[in] ty Translation amount in the y-axis
 * @param[in] tz Translation amount in the z-axis
 * @return Translation matrix created from the given parameters
 */
inline Matrix4 CreateTranslation(float tx, float ty, float tz)
{
    Matrix4 ret = Matrix4::CreateIdentity();
    ret.values[12] = tx;
    ret.values[13] = ty;
    ret.values[14] = tz;
    return ret;
}

/**
 * @brief Creates an orthographic matrix.
 * @param[in] left Left boundary of the viewing box
 * @param[in] right Right boundary of the viewing box
 * @param[in] bottom Bottom boundary of the viewing box
 * @param[in] top Top boundary of the viewing box
 */
inline Matrix4 CreateOrtho(float left, float right, float bottom, float top, float near, float far)
{
	Matrix4 ret = {};

	ret.values[0] = 2.0f / (right - left);
    ret.values[1] = ret.values[2] = ret.values[3] = 0.0f;

    ret.values[5] = 2.0f / (top - bottom);
    ret.values[4] = ret.values[6] = ret.values[7] = 0.0f;

    ret.values[10] = -2.0f / (far - near);
    ret.values[8] = ret.values[9] = ret.values[11] = 0.0f;

    ret.values[12] = -(right + left) / (right - left);
    ret.values[13] = -(top + bottom) / (top - bottom);
    ret.values[14] = -(far + near) / (far - near);
    ret.values[15] = 1.0f;

	return ret;
}
}
