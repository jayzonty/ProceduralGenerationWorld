#include "Mesh.hpp"

#include <cstddef>
#include <iostream>

/**
 * @brief Constructor
 */
Mesh::Mesh()
	: vbo(0)
	, vao(0)
	, ebo(0)
	, vertices()
	, indices()
{
}

/**
 * @brief Destructor
 */
Mesh::~Mesh()
{
	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	if (ebo != 0)
	{
		glDeleteBuffers(1, &ebo);
		ebo = 0;
	}

	if (vao != 0)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
}

/**
 * @brief Draws the mesh
 */
void Mesh::Draw()
{
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
}
