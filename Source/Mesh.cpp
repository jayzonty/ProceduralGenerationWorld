#include "Mesh.hpp"

#include <cstddef>
#include <iostream>

/**
 * @brief Constructor
 */
Mesh::Mesh()
	: m_vbo(0)
	, m_vao(0)
	, m_ebo(0)
	, m_numIndices(0)
{
}

/**
 * @brief Destructor
 */
Mesh::~Mesh()
{
	if (m_vbo != 0)
	{
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
	}

	if (m_ebo != 0)
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}

	if (m_vao != 0)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
}

/**
 * @brief Draws the mesh
 */
void Mesh::Draw()
{
	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_numIndices), GL_UNSIGNED_INT, nullptr);
}
