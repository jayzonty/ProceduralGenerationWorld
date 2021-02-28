#include "Chunk.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "Constants.hpp"

/// <summary>
/// Constructor
/// </summary>
/// <param name="chunkIndexX">Chunk x-index</param>
/// <param name="chunkIndexY">Chunk y-index</param>
Chunk::Chunk(const int& chunkIndexX, const int& chunkIndexZ)
	: m_mesh()
	, m_blocks()
	, m_chunkIndex(chunkIndexX, 0, chunkIndexZ)
{
	int size = Constants::CHUNK_WIDTH * Constants::CHUNK_DEPTH * Constants::CHUNK_HEIGHT;
	m_blocks.resize(size);
}

/// <summary>
/// Destructor
/// </summary>
Chunk::~Chunk()
{
	for (size_t i = 0; i < m_blocks.size(); ++i)
	{
		delete m_blocks[i];
	}
	m_blocks.clear();
}

/// <summary>
	/// Gets the x chunk index for this chunk
	/// </summary>
	/// <returns></returns>
int Chunk::GetChunkIndexX() const
{
	return m_chunkIndex.x;
}

/// <summary>
/// Gets the y chunk index for this chunk
/// </summary>
/// <returns>Chunk y-index</returns>
int Chunk::GetChunkIndexY() const
{
	return m_chunkIndex.y;
}

/// <summary>
/// Gets the z chunk index for this chunk
/// </summary>
/// <returns></returns>
int Chunk::GetChunkIndexZ() const
{
	return m_chunkIndex.z;
}

/// <summary>
/// Gets the chunk indices in each axis for this chunk
/// </summary>
/// <returns>Chunk indices for each axis</returns>
glm::ivec3 Chunk::GetChunkIndices() const
{
	return m_chunkIndex;
}

/// <summary>
/// Generates the mesh for this chunk
/// </summary>
void Chunk::GenerateMesh()
{
	float blockSize = Constants::BLOCK_SIZE;

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec4> vertexColors;
	std::vector<glm::vec2> vertexUVs;
	std::vector<GLuint> indices;
	glm::vec3 origin(m_chunkIndex.x * Constants::CHUNK_WIDTH * blockSize, 0.0f, m_chunkIndex.z * Constants::CHUNK_DEPTH * blockSize * 1.0f);
	for (int x = 0; x < Constants::CHUNK_WIDTH; ++x)
	{
		for (int z = 0; z < Constants::CHUNK_DEPTH; ++z)
		{
			for (int y = 0; y < Constants::CHUNK_HEIGHT; ++y)
			{
				Block* currentBlock = GetBlockAt(x, y, z);
				if (currentBlock != nullptr)
				{
					glm::vec4 uvRects[6]; // Top, Bottom, Left, Right, Front, Back
					if (currentBlock->GetBlockType() == BlockType::Dirt)
					{
						float uvSize = 32.0f / 256.0f;
						uvRects[0] = glm::vec4(0.0f / 256.0f, 224.0f / 256.0f, uvSize, uvSize);
						uvRects[1] = glm::vec4(64.0f / 256.0f, 224.0f / 256.0f, uvSize, uvSize);
						uvRects[2] = uvRects[3] = uvRects[4] = uvRects[5] = glm::vec4(32.0f / 256.0f, 224.0f / 256.0f, uvSize, uvSize);
					}

					// Vertex order: lower-left, lower-right, upper-right, upper-left
					
					// Top face
					if ((y == Constants::CHUNK_HEIGHT - 1) || (GetBlockAt(x, y + 1, z) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, z * blockSize));
						
						float color = 0.75f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[0].x, uvRects[0].y);
						vertexUVs.emplace_back(uvRects[0].x + uvRects[0].z, uvRects[0].y);
						vertexUVs.emplace_back(uvRects[0].x + uvRects[0].z, uvRects[0].y + uvRects[0].w);
						vertexUVs.emplace_back(uvRects[0].x, uvRects[0].y + uvRects[0].w);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Bottom face
					if ((y == 0) || (GetBlockAt(x, y - 1, z) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, (z + 1) * blockSize));
						
						float color = 0.35f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[1].x, uvRects[1].y);
						vertexUVs.emplace_back(uvRects[1].x + uvRects[1].z, uvRects[1].y);
						vertexUVs.emplace_back(uvRects[1].x + uvRects[1].z, uvRects[1].y + uvRects[1].w);
						vertexUVs.emplace_back(uvRects[1].x, uvRects[1].y + uvRects[1].w);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}
					
					// Left face
					if ((x == Constants::CHUNK_WIDTH - 1) || (GetBlockAt(x + 1, y, z) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						
						float color = 0.6f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[2].x, uvRects[2].y);
						vertexUVs.emplace_back(uvRects[2].x + uvRects[2].z, uvRects[2].y);
						vertexUVs.emplace_back(uvRects[2].x + uvRects[2].z, uvRects[2].y + uvRects[2].w);
						vertexUVs.emplace_back(uvRects[2].x, uvRects[2].y + uvRects[2].w);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Right face
					if ((x == 0) || (GetBlockAt(x - 1, y, z) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, z * blockSize));
						
						float color = 0.6f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[3].x, uvRects[3].y);
						vertexUVs.emplace_back(uvRects[3].x + uvRects[3].z, uvRects[3].y);
						vertexUVs.emplace_back(uvRects[3].x + uvRects[3].z, uvRects[3].y + uvRects[3].w);
						vertexUVs.emplace_back(uvRects[3].x, uvRects[3].y + uvRects[3].w);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Front face
					if ((z == 0) || (GetBlockAt(x, y, z - 1) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, z * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, z * blockSize));
						
						float color = 0.5f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[4].x, uvRects[4].y);
						vertexUVs.emplace_back(uvRects[4].x + uvRects[4].z, uvRects[4].y);
						vertexUVs.emplace_back(uvRects[4].x + uvRects[4].z, uvRects[4].y + uvRects[4].w);
						vertexUVs.emplace_back(uvRects[4].x, uvRects[4].y + uvRects[4].w);

						indices.push_back(indexStart);
						indices.push_back(indexStart + 1);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 2);
						indices.push_back(indexStart + 3);
						indices.push_back(indexStart);
					}

					// Back face
					if ((z == Constants::CHUNK_DEPTH - 1) || (GetBlockAt(x, y, z + 1) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(vertexPositions.size());

						vertexPositions.push_back(origin + glm::vec3(x * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, y * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						vertexPositions.push_back(origin + glm::vec3(x * blockSize, (y + 1) * blockSize, (z + 1) * blockSize));
						
						float color = 0.5f;
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);
						vertexColors.emplace_back(color, color, color, 1.0f);

						vertexUVs.emplace_back(uvRects[5].x, uvRects[5].y);
						vertexUVs.emplace_back(uvRects[5].x + uvRects[5].z, uvRects[5].y);
						vertexUVs.emplace_back(uvRects[5].x + uvRects[5].z, uvRects[5].y + uvRects[5].w);
						vertexUVs.emplace_back(uvRects[5].x, uvRects[5].y + uvRects[5].w);

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
	m_mesh.SetVertexUVs(vertexUVs);
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
Block* Chunk::GetBlockAt(int x, int y, int z)
{
	int index = (z * Constants::CHUNK_WIDTH + x) * Constants::CHUNK_HEIGHT + y;
	return m_blocks[index];
}

/// <summary>
/// Sets the block at the specified location with the specified block
/// </summary>
/// <param name="x">X-coordinate</param>
/// <param name="y">Y-coordinate</param>
/// <param name="z">Z-coordinate</param>
/// <param name="block">Data for the new block. Can be set to nullptr if it's an air block.</param>
void Chunk::SetBlockAt(const int& x, const int& y, const int& z, Block* block)
{
	Block* existingBlock = GetBlockAt(x, y, z);
	if (existingBlock != nullptr)
	{
		delete existingBlock;
	}

	int index = (z * Constants::CHUNK_WIDTH + x) * Constants::CHUNK_HEIGHT + y;
	m_blocks[index] = block;
}
