#include "Mesh.hpp"

#include <cstddef>
#include <iostream>

/// <summary>
/// Constructor
/// </summary>
Mesh::Mesh()
	: m_vbo(0)
	, m_vao(0)
	, m_ebo(0)
	, m_positions()
	, m_colors()
	, m_indices()
{
}

/// <summary>
/// Destructor
/// </summary>
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

/// <summary>
/// Generates the mesh
/// </summary>
void Mesh::GenerateMesh()
{
	if (m_vao == 0)
	{
		glGenVertexArrays(1, &m_vao);
	}
	glBindVertexArray(m_vao);

	if (m_vbo == 0)
	{
		glGenBuffers(1, &m_vbo);
	}

	std::vector<Vertex> vertices;
	for (size_t i = 0; i < m_positions.size(); ++i)
	{
		vertices.emplace_back();
		vertices.back().x = m_positions[i].x;
		vertices.back().y = m_positions[i].y;
		vertices.back().z = m_positions[i].z;
		vertices.back().r = m_colors[i].x;
		vertices.back().g = m_colors[i].y;
		vertices.back().b = m_colors[i].z;
		vertices.back().a = m_colors[i].w;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

	if (m_ebo == 0)
	{
		glGenBuffers(1, &m_ebo);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, x)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, r)));

	glBindVertexArray(0);
}

/// <summary>
/// Draws the mesh
/// </summary>
void Mesh::Draw()
{
	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
}

/// <summary>
/// Sets list of vertex positions
/// </summary>
/// <param name="positions">New list of vertex positions</param>
void Mesh::SetVertexPositions(const std::vector<glm::vec3>& positions)
{
	m_positions = positions;
}

/// <summary>
/// Sets list of vertex colors
/// </summary>
/// <param name="colors">New list of vertex colors</param>
void Mesh::SetVertexColors(const std::vector<glm::vec4>& colors)
{
	m_colors = colors;
}

/// <summary>
/// Sets list of vertex indices
/// </summary>
/// <param name="indices">New list of vertex indices</param>
void Mesh::SetIndices(const std::vector<GLuint>& indices)
{
	m_indices = indices;
}
