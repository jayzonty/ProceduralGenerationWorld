#include "Chunk.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

/// <summary>
/// Constructor
/// </summary>
/// <param name="chunkIndexX">Chunk x-index</param>
/// <param name="chunkIndexY">Chunk y-index</param>
Chunk::Chunk(const int& chunkIndexX, const int& chunkIndexZ)
	: m_mesh()
	, m_blocks()
	, m_chunkIndexX(chunkIndexX)
	, m_chunkIndexZ(chunkIndexZ)
{
	int size = CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT;
	m_blocks.resize(size);
}

/// <summary>
/// Destructor
/// </summary>
Chunk::~Chunk()
{
}

/// <summary>
	/// Gets the x chunk index for this chunk
	/// </summary>
	/// <returns></returns>
int Chunk::GetChunkIndexX() const
{
	return m_chunkIndexX;
}

/// <summary>
/// Gets the z chunk index for this chunk
/// </summary>
/// <returns></returns>
int Chunk::GetChunkIndexZ() const
{
	return m_chunkIndexZ;
}

/// <summary>
/// Generates the mesh for this chunk
/// </summary>
void Chunk::GenerateMesh()
{
	float blockSize = 1.0f;
	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec4> vertexColors;
	std::vector<GLuint> indices;
	glm::vec3 origin(m_chunkIndexX * CHUNK_WIDTH * blockSize, 0.0f, m_chunkIndexZ * CHUNK_DEPTH * blockSize * -1.0f);
	for (int x = 0; x < CHUNK_WIDTH; ++x)
	{
		for (int z = 0; z < CHUNK_DEPTH; ++z)
		{
			for (int y = 0; y < CHUNK_HEIGHT; ++y)
			{
				if (GetBlockAt(x, y, z).GetBlockType() == BlockType::Dirt)
				{
					// Top face
					if ((y == CHUNK_HEIGHT - 1) || (GetBlockAt(x, y + 1, z).GetBlockType() == BlockType::Air))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, z * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, z * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, (z + 1) * -blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, (z + 1) * -blockSize));

						float color = 0.75f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Bottom face
					if ((y == 0) || (GetBlockAt(x, y - 1, z).GetBlockType() == BlockType::Air))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, z * -blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, (z + 1) * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, (z + 1) * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, z * -blockSize));

						float color = 0.35f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}
					
					// Left face
					if ((x == 0) || (GetBlockAt(x - 1, y, z).GetBlockType() == BlockType::Air))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, z * -blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, (z + 1) * -blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, (z + 1) * -blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, z * -blockSize));

						float color = 0.6f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Right face
					if ((x == CHUNK_WIDTH - 1) || (GetBlockAt(x + 1, y, z).GetBlockType() == BlockType::Air))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, z * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, z * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, (z + 1) * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, (z + 1) * -blockSize));

						float color = 0.6f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Front face
					if ((z == CHUNK_DEPTH - 1) || (GetBlockAt(x, y, z + 1).GetBlockType() == BlockType::Air))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, (z + 1) * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, (z + 1) * -blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, (z + 1) * -blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, (z + 1) * -blockSize));

						float color = 0.5f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Back face
					if ((z == 0) || (GetBlockAt(x, y, z - 1).GetBlockType() == BlockType::Air))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, z * -blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, z * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, z * -blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, z * -blockSize));

						float color = 0.5f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}
				}
			}
		}
	}

	m_mesh.SetVertexPositions(vertexPositions);
	m_mesh.SetVertexColors(vertexColors);
	m_mesh.SetIndices(indices);
	m_mesh.GenerateMesh();
}

/// <summary>
/// Draw the chunk
/// </summary>
void Chunk::Draw()
{
	m_mesh.Draw();
}

/// <summary>
/// Gets the block at the specified location
/// </summary>
/// <param name="x">X-coordinate</param>
/// <param name="y">Y-coordinate</param>
/// <param name="z">Z-coordinate</param>
/// <returns></returns>
Block& Chunk::GetBlockAt(size_t x, size_t y, size_t z)
{
	size_t index = (z * CHUNK_WIDTH + x) * CHUNK_HEIGHT + y;
	return m_blocks[index];
}
