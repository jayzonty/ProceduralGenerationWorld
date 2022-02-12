#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

/**
 * Mesh class
 */
class Mesh
{
private:
	friend class MeshBuilder;

	/**
	 * VBO handle
	 */
	GLuint m_vbo;

	/**
	 * VAO handle
	 */
	GLuint m_vao;

	/**
	 * EBO handle
	 */
	GLuint m_ebo;

	/**
	 * Number of indices
	 */
	int m_numIndices;

public:
	/**
	 * @brief Constructor
	 */
	Mesh();

	/**
	 * @brief Destructor
	 */
	~Mesh();

	/**
	 * @brief Draws the mesh
	 */
	void Draw();
};
