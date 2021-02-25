#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "Shader.hpp"

/// <summary>
/// Mesh class
/// </summary>
class Mesh
{
private:
	/// <summary>
	/// VBO handle
	/// </summary>
	GLuint m_vbo;

	/// <summary>
	/// VAO handle
	/// </summary>
	GLuint m_vao;

	/// <summary>
	/// EBO handle
	/// </summary>
	GLuint m_ebo;

	/// <summary>
	/// Struct containing vertex data
	/// </summary>
	struct Vertex
	{
		/// <summary>
		/// Position
		/// </summary>
		GLfloat x, y, z;

		/// <summary>
		/// Color
		/// </summary>
		GLfloat r, g, b, a;
	};

	/// <summary>
	/// List of vertex positions
	/// </summary>
	std::vector<glm::vec3> m_positions;

	/// <summary>
	/// List of vertex colors
	/// </summary>
	std::vector<glm::vec4> m_colors;

	/// <summary>
	/// List of indices
	/// </summary>
	std::vector<GLuint> m_indices;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	Mesh();

	/// <summary>
	/// Destructor
	/// </summary>
	~Mesh();

	/// <summary>
	/// Generates the mesh
	/// </summary>
	void GenerateMesh();

	/// <summary>
	/// Draws the mesh
	/// </summary>
	void Draw();

	/// <summary>
	/// Sets list of vertex positions
	/// </summary>
	/// <param name="positions">New list of vertex positions</param>
	void SetVertexPositions(const std::vector<glm::vec3>& positions);

	/// <summary>
	/// Sets list of vertex colors
	/// </summary>
	/// <param name="colors">New list of vertex colors</param>
	void SetVertexColors(const std::vector<glm::vec4>& colors);

	/// <summary>
	/// Sets list of vertex indices
	/// </summary>
	/// <param name="indices">New list of vertex indices</param>
	void SetIndices(const std::vector<GLuint>& indices);
};
