#include "MeshBuilder.hpp"

/// <summary>
/// Constructor
/// </summary>
MeshBuilder::MeshBuilder()
	: m_positions()
	, m_colors()
	, m_uvCoordinates()
	, m_normals()
	, m_indices()
{
}

/// <summary>
/// Destructor
/// </summary>
MeshBuilder::~MeshBuilder()
{
}

/// <summary>
/// Builds the mesh using prior provided information,
/// and stores the result to the provided mesh reference.
/// </summary>
/// <param name="mesh">Reference to the mesh where the result will be stored</param>
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
		vertices.back().r = m_colors[i].x;
		vertices.back().g = m_colors[i].y;
		vertices.back().b = m_colors[i].z;
		vertices.back().a = m_colors[i].w;
		vertices.back().u = m_uvCoordinates[i].x;
		vertices.back().v = m_uvCoordinates[i].y;
		vertices.back().nx = m_normals[i].x;
		vertices.back().ny = m_normals[i].y;
		vertices.back().nz = m_normals[i].z;
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

/// <summary>
/// Sets list of vertex positions
/// </summary>
/// <param name="positions">New list of vertex positions</param>
void MeshBuilder::SetVertexPositions(const std::vector<glm::vec3>& positions)
{
	m_positions = positions;
}

/// <summary>
/// Sets list of vertex colors
/// </summary>
/// <param name="colors">New list of vertex colors</param>
void MeshBuilder::SetVertexColors(const std::vector<glm::vec4>& colors)
{
	m_colors = colors;
}

/// <summary>
/// Sets list of vertex UV coordinates
/// </summary>
/// <param name="uvCoordinates">New list of vertex UV coordinates</param>
void MeshBuilder::SetVertexUVs(const std::vector<glm::vec2>& uvCoordinates)
{
	m_uvCoordinates = uvCoordinates;
}

/// <summary>
/// Sets list of vertex normal vectors
/// </summary>
/// <param name="normals">New list of vertex normal vectors</param>
void MeshBuilder::SetVertexNormals(const std::vector<glm::vec3>& normals)
{
	m_normals = normals;
}

/// <summary>
/// Sets list of vertex indices
/// </summary>
/// <param name="indices">New list of vertex indices</param>
void MeshBuilder::SetIndices(const std::vector<GLuint>& indices)
{
	m_indices = indices;
}
