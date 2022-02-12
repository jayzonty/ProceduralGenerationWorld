#include "MeshBuilder.hpp"

/**
 * @brief Constructor
 */
MeshBuilder::MeshBuilder()
	: m_positions()
	, m_colors()
	, m_uvCoordinates()
	, m_normals()
	, m_indices()
{
}

/**
 * @brief Destructor
 */
MeshBuilder::~MeshBuilder()
{
}

/**
 * @brief Clear all stored data
 */
void MeshBuilder::Clear()
{
	m_positions.clear();
	m_colors.clear();
	m_uvCoordinates.clear();
	m_normals.clear();
	m_indices.clear();
}

/**
 * @brief Sets list of vertex positions
 * @param[in] positions New list of vertex positions
 */
void MeshBuilder::SetVertexPositions(const std::vector<glm::vec3>& positions)
{
	m_positions = positions;
}

/**
 * @brief Sets list of vertex colors
 * @param[in] colors New list of vertex colors
 */
void MeshBuilder::SetVertexColors(const std::vector<glm::vec4>& colors)
{
	m_colors = colors;
}

/**
 * @brief Sets list of vertex UV coordinates
 * @param[in] uvCoordinates New list of vertex UV coordinates
 */
void MeshBuilder::SetVertexUVs(const std::vector<glm::vec2>& uvCoordinates)
{
	m_uvCoordinates = uvCoordinates;
}

/**
 * @brief Sets list of vertex normal vectors
 * @param[in] normals New list of vertex normal vectors
 */
void MeshBuilder::SetVertexNormals(const std::vector<glm::vec3>& normals)
{
	m_normals = normals;
}

/**
 * @brief Sets list of vertex indices
 * @param[in] indices New list of vertex indices
 */
void MeshBuilder::SetIndices(const std::vector<GLuint>& indices)
{
	m_indices = indices;
}

/**
 * @brief Builds the mesh using prior provided information,
 * and stores the result to the provided mesh reference.
 * @param[in] mesh Reference to the mesh where the result will be stored
 */
void MeshBuilder::BuildMesh(Mesh& mesh)
{
	if (mesh.m_vao == 0)
	{
		glGenVertexArrays(1, &mesh.m_vao);
	}
	glBindVertexArray(mesh.m_vao);

	if (mesh.m_vbo == 0)
	{
		glGenBuffers(1, &mesh.m_vbo);
	}

	std::vector<Vertex> vertices;
	for (size_t i = 0; i < m_positions.size(); ++i)
	{
		vertices.emplace_back();

		vertices.back().x = m_positions[i].x;
		vertices.back().y = m_positions[i].y;
		vertices.back().z = m_positions[i].z;

		if (i < m_colors.size())
		{
			vertices.back().r = m_colors[i].r;
			vertices.back().g = m_colors[i].g;
			vertices.back().b = m_colors[i].b;
			vertices.back().a = m_colors[i].a;
		}
		
		if (i < m_uvCoordinates.size())
		{
			vertices.back().u = m_uvCoordinates[i].x;
			vertices.back().v = m_uvCoordinates[i].y;
		}
		
		if (i < m_normals.size())
		{
			vertices.back().nx = m_normals[i].x;
			vertices.back().ny = m_normals[i].y;
			vertices.back().nz = m_normals[i].z;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

	if (mesh.m_ebo == 0)
	{
		glGenBuffers(1, &mesh.m_ebo);
	}

	mesh.m_numIndices = static_cast<int>(m_indices.size());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, x)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, r)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, u)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, nx)));

	glBindVertexArray(0);
}
