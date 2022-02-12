#pragma once

#include "Mesh.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

/**
 * Class for handling building of a mesh
 * and uploading to the GPU
 */
class MeshBuilder
{
private:
	/**
	 * Struct containing vertex data
	 */
	struct Vertex
	{
		/**
		 * Position
		 */
		GLfloat x, y, z;

		/**
		 * Color
		 */
		GLfloat r, g, b, a;

		/**
		 * Texture coordinates
		 */
		GLfloat u, v;

		/**
		 * Normal
		 */
		GLfloat nx, ny, nz;
	};

	/**
	 * List of vertex positions
	 */
	std::vector<glm::vec3> m_positions;

	/**
	 * List of vertex colors
	 */
	std::vector<glm::vec4> m_colors;

	/**
	 * List of UV coordinates
	 */
	std::vector<glm::vec2> m_uvCoordinates;

	/**
	 * List of vertex normals
	 */
	std::vector<glm::vec3> m_normals;

	/**
	 * List of indices
	 */
	std::vector<GLuint> m_indices;

public:
	/**
	 * @brief Constructor
	 */
	MeshBuilder();

	/**
	 * @brief Destructor
	 */
	~MeshBuilder();

	/**
	 * @brief Clear all stored data
	 */
	void Clear();

	/**
	 * @brief Sets list of vertex positions
	 * @param[in] positions New list of vertex positions
	 */
	void SetVertexPositions(const std::vector<glm::vec3>& positions);

	/**
	 * @brief Sets list of vertex colors
	 * @param[in] colors New list of vertex colors
	 */
	void SetVertexColors(const std::vector<glm::vec4>& colors);

	/**
	 * @brief Sets list of vertex UV coordinates
	 * @param[in] uvCoordinates New list of vertex UV coordinates
	 */
	void SetVertexUVs(const std::vector<glm::vec2>& uvCoordinates);

	/**
	 * @brief Sets list of vertex normal vectors
	 * @param[in] normals New list of vertex normal vectors
	 */
	void SetVertexNormals(const std::vector<glm::vec3>& normals);

	/**
	 * @brief Sets list of vertex indices
	 * @param[in] indices New list of vertex indices
	 */
	void SetIndices(const std::vector<GLuint>& indices);

	/**
	 * @brief Builds the mesh using prior provided information,
	 * and stores the result to the provided mesh reference.
	 * @param[in] mesh Reference to the mesh where the result will be stored
	 */
	void BuildMesh(Mesh& mesh);
};
