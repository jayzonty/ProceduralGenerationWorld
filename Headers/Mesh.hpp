#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "ShaderProgram.hpp"
#include "Vertex.hpp"

/**
 * Mesh class
 */
struct Mesh
{
	/**
	 * VBO handle
	 */
	GLuint vbo;

	/**
	 * VAO handle
	 */
	GLuint vao;

	/**
	 * EBO handle
	 */
	GLuint ebo;

	/**
	 * Vertex list
	 */
	std::vector<Vertex> vertices;

	/**
	 * Index list
	 */
	std::vector<GLuint> indices;

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
