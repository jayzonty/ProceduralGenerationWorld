#pragma once

#include "Mesh.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

/// <summary>
/// Class for handling building of a mesh
/// and uploading to the GPU
/// </summary>
class MeshBuilder
{
private:
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

		/// <summary>
		/// Texture coordinates
		/// </summary>
		GLfloat u, v;
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
	/// List of UV coordinates
	/// </summary>
	std::vector<glm::vec2> m_uvCoordinates;

	/// <summary>
	/// List of indices
	/// </summary>
	std::vector<GLuint> m_indices;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	MeshBuilder();

	/// <summary>
	/// Destructor
	/// </summary>
	~MeshBuilder();

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
	/// Sets list of vertex UV coordinates
	/// </summary>
	/// <param name="uvCoordinates">New list of vertex UV coordinates</param>
	void SetVertexUVs(const std::vector<glm::vec2>& uvCoordinates);

	/// <summary>
	/// Sets list of vertex indices
	/// </summary>
	/// <param name="indices">New list of vertex indices</param>
	void SetIndices(const std::vector<GLuint>& indices);

	/// <summary>
	/// Builds the mesh using prior provided information,
	/// and stores the result to the provided mesh reference.
	/// </summary>
	/// <param name="mesh">Reference to the mesh where the result will be stored</param>
	void BuildMesh(Mesh& mesh);
};
