#pragma once

#include "Engine/ECS/ECS.hpp"
#include <cstdint>

namespace Engine
{
struct SceneComponent
{
    ECS::entity_t parent;

    uint32_t indexInHierarchy;
};
}
