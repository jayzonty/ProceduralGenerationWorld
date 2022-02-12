#pragma once

#include "Engine/Matrix.hpp"
#include "Engine/Vector.hpp"

namespace Engine
{
namespace ECS
{
/**
 * Component struct containing data for an object's transform
 */
struct TransformComponent
{
    /**
     * Parent transform
     */
    TransformComponent* parent;

    /**
     * Local position
     */
    Vector2f localPosition;

    /**
     * Local depth
     */
    int32_t localDepth = 0;

    /**
     * @brief Gets the world position of this transform.
     * @return World position of this transform
     */
    Vector2f GetWorldPosition() const
    {
        Vector2f ret = localPosition;

        TransformComponent* currentParent = parent;
        while (currentParent != nullptr)
        {
            ret = ret + currentParent->localPosition;
            currentParent = currentParent->parent;
        }

        return ret;
    }

    /**
     * @brief Sets the world position of this transform.
     * @param[in] newPosition New world position
     */
    void SetWorldPosition(const Vector2f& newPosition)
    {
        localPosition = newPosition;
        
        TransformComponent* currentParent = parent;
        while (currentParent != nullptr)
        {
            localPosition = localPosition - currentParent->localPosition;
            currentParent = currentParent->parent;
        }
    }

    /**
     * @brief Gets the depth of this transform in world space
     * @return Depth in world space
     */
    int32_t GetWorldDepth() const
    {
        int32_t ret = localDepth;

        TransformComponent* currentParent = parent;
        while (currentParent != nullptr)
        {
            ret = ret + currentParent->localDepth;
            currentParent = currentParent->parent;
        }

        return ret;
    }
};
}
}
