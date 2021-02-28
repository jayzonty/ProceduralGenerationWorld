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
	friend class MeshBuilder;

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
	/// Number of indices
	/// </summary>
	int m_numIndices;

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
	/// Draws the mesh
	/// </summary>
	void Draw();
};
