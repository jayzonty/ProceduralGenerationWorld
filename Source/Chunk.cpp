#include "Chunk.hpp"

#include <array>
#include <cstddef>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "BlockUtils.hpp"
#include "Constants.hpp"
#include "Enums/BlockFaceEnum.hpp"
#include "ResourceManager.hpp"

#include "Enums/BlockTypeEnum.hpp"
#include "EntityTemplates/BlockTemplate.hpp"
#include "EntityTemplates/BlockTemplateManager.hpp"
#include "glm/fwd.hpp"

/**
 * @brief Constructor
 * @param[in] chunkIndexX Chunk x-index
 * @param[in] chunkIndexZ Chunk z-index
 */
Chunk::Chunk(const int& chunkIndexX, const int& chunkIndexZ)
	: m_terrainMesh()
	, m_waterMesh()
	, m_blocks()
	, m_chunkIndex(chunkIndexX, 0, chunkIndexZ)
{
	int size = Constants::CHUNK_WIDTH * Constants::CHUNK_DEPTH * Constants::CHUNK_HEIGHT;
	m_blocks.resize(size);
}

/**
 * @brief Destructor
 */
Chunk::~Chunk()
{
	for (size_t i = 0; i < m_blocks.size(); ++i)
	{
		delete m_blocks[i];
	}
	m_blocks.clear();
}

/**
 * @brief Gets the x chunk index for this chunk
 * @return Chunk x-index
 */
int Chunk::GetChunkIndexX() const
{
	return m_chunkIndex.x;
}

/**
 * @brief Gets the y chunk index for this chunk
 * @return Chunk y-index
 */
int Chunk::GetChunkIndexY() const
{
	return m_chunkIndex.y;
}

/**
 * @brief Gets the z chunk index for this chunk
 * @return Chunk z-index
 */
int Chunk::GetChunkIndexZ() const
{
	return m_chunkIndex.z;
}

/**
 * @brief Gets the chunk indices in each axis for this chunk
 * @return Chunk indices for each axis
 */
glm::ivec3 Chunk::GetChunkIndices() const
{
	return m_chunkIndex;
}

void AddFaceVerticesToMesh(const BlockFaceEnum &face, const BlockTemplate *blockTemplate, const glm::vec3 &origin, Mesh &mesh)
{
	std::array<glm::vec3, 4> offsets = BlockUtils::GetVertexOffsetsFromFace(face);

	glm::vec4 color = BlockUtils::GetColorTintFromFace(face);

	glm::vec4 uvRect = blockTemplate->GetFaceUVRect(face);
	std::array<glm::vec2, 4> uvs =
	{
		glm::vec2 { uvRect.x, uvRect.y },
		glm::vec2 { uvRect.x + uvRect.z, uvRect.y },
		glm::vec2 { uvRect.x + uvRect.z, uvRect.y + uvRect.w },
		glm::vec2 { uvRect.x, uvRect.y + uvRect.w }
	};

	glm::vec3 normal = BlockUtils::GetNormalFromFace(face);

	GLuint indexStart = mesh.vertices.size();
	for (size_t i = 0; i < 4; ++i)
	{
		mesh.vertices.emplace_back();
		mesh.vertices.back().position = origin + offsets[i] * Constants::BLOCK_SIZE;
		mesh.vertices.back().color = color;
		mesh.vertices.back().uv = uvs[i];
		mesh.vertices.back().normal = normal;
	}

	mesh.indices.push_back(indexStart + 0);
	mesh.indices.push_back(indexStart + 1);
	mesh.indices.push_back(indexStart + 2);
	mesh.indices.push_back(indexStart + 2);
	mesh.indices.push_back(indexStart + 3);
	mesh.indices.push_back(indexStart + 0);
}

/**
 * @brief Generates the mesh for this chunk
 */
void Chunk::GenerateMesh()
{
	m_terrainMesh.vertices.clear();
	m_terrainMesh.indices.clear();

	float blockSize = Constants::BLOCK_SIZE;

	glm::vec3 origin(m_chunkIndex.x * Constants::CHUNK_WIDTH * blockSize, 0.0f, m_chunkIndex.z * Constants::CHUNK_DEPTH * blockSize * 1.0f);
	for (int x = 0; x < Constants::CHUNK_WIDTH; ++x)
	{
		for (int z = 0; z < Constants::CHUNK_DEPTH; ++z)
		{
			for (int y = 0; y < Constants::CHUNK_HEIGHT; ++y)
			{
				glm::vec3 blockOrigin(origin.x + x * blockSize, origin.y + y * blockSize, origin.z + z * blockSize);
				Block* currentBlock = GetBlockAt(x, y, z);
				if (currentBlock != nullptr && currentBlock->GetBlockType() != BlockTypeEnum::WATER)
				{
					const BlockTemplate* blockTemplate = BlockTemplateManager::GetInstance().GetBlockTemplate(currentBlock->GetBlockType());

					// Top face
					if ((y == Constants::CHUNK_HEIGHT - 1) || ((GetBlockAt(x, y + 1, z) == nullptr) || (GetBlockAt(x, y + 1, z)->GetBlockType() == BlockTypeEnum::WATER)))
					{
						AddFaceVerticesToMesh(BlockFaceEnum::TOP, blockTemplate, blockOrigin, m_terrainMesh);
					}

					// Bottom face
					if ((y == 0) || (GetBlockAt(x, y - 1, z) == nullptr))
					{
						AddFaceVerticesToMesh(BlockFaceEnum::BOTTOM, blockTemplate, blockOrigin, m_terrainMesh);
					}
					
					// Left face
					if ((x == Constants::CHUNK_WIDTH - 1) || (GetBlockAt(x + 1, y, z) == nullptr))
					{
						AddFaceVerticesToMesh(BlockFaceEnum::LEFT, blockTemplate, blockOrigin, m_terrainMesh);
					}

					// Right face
					if ((x == 0) || (GetBlockAt(x - 1, y, z) == nullptr))
					{
						AddFaceVerticesToMesh(BlockFaceEnum::RIGHT, blockTemplate, blockOrigin, m_terrainMesh);
					}

					// Front face
					if ((z == 0) || (GetBlockAt(x, y, z - 1) == nullptr))
					{
						AddFaceVerticesToMesh(BlockFaceEnum::FRONT, blockTemplate, blockOrigin, m_terrainMesh);
					}

					// Back face
					if ((z == Constants::CHUNK_DEPTH - 1) || (GetBlockAt(x, y, z + 1) == nullptr))
					{
						AddFaceVerticesToMesh(BlockFaceEnum::BACK, blockTemplate, blockOrigin, m_terrainMesh);
					}
				}
			}
		}
	}

	if (m_terrainMesh.vao == 0)
	{
		glGenVertexArrays(1, &m_terrainMesh.vao);
	}
	glBindVertexArray(m_terrainMesh.vao);

	if (m_terrainMesh.vbo == 0)
	{
		glGenBuffers(1, &m_terrainMesh.vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_terrainMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_terrainMesh.vertices.size(), m_terrainMesh.vertices.data(), GL_DYNAMIC_DRAW);

	if (m_terrainMesh.ebo == 0)
	{
		glGenBuffers(1, &m_terrainMesh.ebo);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_terrainMesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_terrainMesh.indices.size(), m_terrainMesh.indices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

	glBindVertexArray(0);

	// Generate water mesh
	for (int x = 0; x < Constants::CHUNK_WIDTH; ++x)
	{
		for (int z = 0; z < Constants::CHUNK_DEPTH; ++z)
		{
			for (int y = 0; y < Constants::CHUNK_HEIGHT; ++y)
			{
				Block* currentBlock = GetBlockAt(x, y, z);
				if ((currentBlock != nullptr) && (currentBlock->GetBlockType() == BlockTypeEnum::WATER))
				{
					if ((y == Constants::CHUNK_HEIGHT - 1) || (GetBlockAt(x, y + 1, z) == nullptr))
					{
						GLuint indexStart = static_cast<GLuint>(m_waterMesh.vertices.size());

						float yOffset = -0.1f;
						m_waterMesh.vertices.emplace_back();
						m_waterMesh.vertices.back().position = origin + glm::vec3(x * blockSize, (y + 1) * blockSize + yOffset, (z + 1) * blockSize);
						m_waterMesh.vertices.emplace_back();
						m_waterMesh.vertices.back().position = origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize + yOffset, (z + 1) * blockSize);
						m_waterMesh.vertices.emplace_back();
						m_waterMesh.vertices.back().position = origin + glm::vec3((x + 1) * blockSize, (y + 1) * blockSize + yOffset, z * blockSize);
						m_waterMesh.vertices.emplace_back();
						m_waterMesh.vertices.back().position = origin + glm::vec3(x * blockSize, (y + 1) * blockSize + yOffset, z * blockSize);

						m_waterMesh.indices.push_back(indexStart);
						m_waterMesh.indices.push_back(indexStart + 1);
						m_waterMesh.indices.push_back(indexStart + 2);
						m_waterMesh.indices.push_back(indexStart + 2);
						m_waterMesh.indices.push_back(indexStart + 3);
						m_waterMesh.indices.push_back(indexStart);
					}
				}
			}
		}
	}

	if (m_waterMesh.vao == 0)
	{
		glGenVertexArrays(1, &m_waterMesh.vao);
	}
	glBindVertexArray(m_waterMesh.vao);

	if (m_waterMesh.vbo == 0)
	{
		glGenBuffers(1, &m_waterMesh.vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_waterMesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_waterMesh.vertices.size(), m_waterMesh.vertices.data(), GL_DYNAMIC_DRAW);

	if (m_waterMesh.ebo == 0)
	{
		glGenBuffers(1, &m_waterMesh.ebo);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_waterMesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_waterMesh.indices.size(), m_waterMesh.indices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

	glBindVertexArray(0);
}

/**
 * @brief Gets the mesh for the terrain
 * @return Terrain mesh
 */
Mesh* Chunk::GetTerrainMesh()
{
	return &m_terrainMesh;
}

/**
 * @brief Gets the mesh for water-type blocks
 * @return Mesh for water-type blocks
 */
Mesh* Chunk::GetWaterMesh()
{
	return &m_waterMesh;
}

/**
 * @brief Gets the block at the specified location
 * @param[in] x X-coordinate
 * @param[in] y Y-coordinate
 * @param[in] z z-coordinate
 * @return Block data of the block at the specified location
 */
Block* Chunk::GetBlockAt(int x, int y, int z)
{
	int index = (z * Constants::CHUNK_WIDTH + x) * Constants::CHUNK_HEIGHT + y;
	return m_blocks[index];
}

/**
 * @brief Sets the block at the specified location with the specified block
 * @param[in] x X-coordinate
 * @param[in] y Y-coordinate
 * @param[in] z z-coordinate
 * @param[in] block Data for the new block. Can be set to nullptr if it's an air block.
 */
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
