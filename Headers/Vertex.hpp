#pragma once

#include <glm/glm.hpp>

/**
 * Struct containing data about a vertex
 */
struct Vertex
{
    /**
	 * Position
     */
    glm::vec3 position;

    /**
	 * Color
     */
    glm::vec4 color;

    /**
	 * Texture coordinates
     */
    glm::vec2 uv;

    /**
	 * Normal
     */
    glm::vec3 normal;
};
