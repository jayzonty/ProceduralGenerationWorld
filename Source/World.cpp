#include "World.hpp"

#include "Constants.hpp"
#include "Mesh.hpp"
#include "ResourceManager.hpp"
#include "Utils/NoiseUtils.hpp"

#include <cstdint>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/**
 * @brief Constructor
 */
World::World()
	: m_chunks()
	, m_noiseEngine()
	, m_worldGenParams()
{
	m_worldGenParams.worldSize = 1024;
	m_worldGenParams.worldMaxHeight = 30;

	m_worldGenParams.seed = 0;
	m_worldGenParams.noiseNumOctaves = 1;
	m_worldGenParams.noiseScale = 1.0f;
	m_worldGenParams.noisePersistence = 1.0f;
	m_worldGenParams.noiseLacunarity = 2.0f;
}

/**
 * @brief Destructor
 */
World::~World()
{
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		delete m_chunks[i];
	}
	m_chunks.clear();
}

/**
 * @brief Sets the parameters for the world generation
 * @param[in] params Struct containing the parameters for the world generation
 */
void World::SetWorldGenParams(const WorldGenParams &params)
{
	m_worldGenParams = params;
}

/**
 * @brief Get chunk at the provided location indices
 * @param[in] chunkIndexX Chunk x-index
 * @param[in] chunkIndexZ Chunk z-index
 * @return Chunk at the provided location. Returns nullptr if the chunk has not been generated yet.
 */
Chunk* World::GetChunkAt(const int& chunkIndexX, const int& chunkIndexZ)
{
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		if ((m_chunks[i]->GetChunkIndexX() == chunkIndexX) && (m_chunks[i]->GetChunkIndexZ() == chunkIndexZ))
		{
			return m_chunks[i];
		}
	}

	return nullptr;
}

/**
 * @brief Gets the chunk at the given world position
 * @param[in] worldPosition World position
 * @return Chunk at the given world position
 */
Chunk* World::GetChunkAtWorldPosition(const glm::vec3& worldPosition)
{
	glm::ivec3 chunkIndex = WorldPositionToChunkIndex(worldPosition);
	return GetChunkAt(chunkIndex.x, chunkIndex.z);
}

/**
 * @brief Gets the block at the given world position
 * @param[in] worldPosition World position
 * @return Block at the given world position
 */
Block* World::GetBlockAtWorldPosition(const glm::vec3& worldPosition)
{
	glm::ivec3 chunkIndex = WorldPositionToChunkIndex(worldPosition);
	Chunk* chunk = GetChunkAt(chunkIndex.x, chunkIndex.z);

	glm::vec3 worldPositionCopy = worldPosition / Constants::BLOCK_SIZE;
	glm::ivec3 blockPosition(0);
	blockPosition.x = (static_cast<int>(glm::floor(worldPositionCopy.x)) + (Constants::CHUNK_WIDTH * glm::abs(chunkIndex.x))) % Constants::CHUNK_WIDTH;
	blockPosition.y = (static_cast<int>(glm::floor(worldPositionCopy.y)) + (Constants::CHUNK_HEIGHT * glm::abs(chunkIndex.y))) % Constants::CHUNK_HEIGHT;
	blockPosition.z = (static_cast<int>(glm::floor(worldPositionCopy.z)) + (Constants::CHUNK_DEPTH * glm::abs(chunkIndex.z))) % Constants::CHUNK_DEPTH;

	return chunk->GetBlockAt(blockPosition.x, blockPosition.y, blockPosition.z);
}

/**
 * @brief Converts the provided world position to chunk index
 * @param[in] worldPosition World position
 * @return Chunk index of the provided world position
 */
glm::ivec3 World::WorldPositionToChunkIndex(const glm::vec3& worldPosition)
{
	glm::ivec3 ret(0);
	ret.x = static_cast<int>(glm::floor(worldPosition.x / Constants::BLOCK_SIZE / Constants::CHUNK_WIDTH));
	ret.z = static_cast<int>(glm::floor(worldPosition.z / Constants::BLOCK_SIZE / Constants::CHUNK_DEPTH));
	return ret;
}

/**
 * @brief Generate chunk at the provided location indices
 * @param[in] chunkIndexX Chunk x-index
 * @param[in] chunkIndexZ Chunk z-index
 * @return Generated chunk
 */
Chunk* World::GenerateChunkAt(const int& chunkIndexX, const int& chunkIndexZ)
{
	int32_t worldCenterX = m_worldGenParams.worldSize / 2;
	int32_t worldCenterZ = m_worldGenParams.worldSize / 2;

	int32_t fallOff = 64;
	int32_t outerRadius = m_worldGenParams.worldSize / 2;
	int32_t innerRadius = std::max(outerRadius - fallOff, 0);
	int32_t squareOuterRadius = outerRadius * outerRadius;
	int32_t squareInnerRadius = innerRadius * innerRadius;

	Chunk* chunk = GetChunkAt(chunkIndexX, chunkIndexZ);
	if (chunk == nullptr)
	{
		chunk = new Chunk(chunkIndexX, chunkIndexZ);
		for (int x = 0; x < Constants::CHUNK_WIDTH; ++x)
		{
			for (int z = 0; z < Constants::CHUNK_DEPTH; ++z)
			{
				int32_t blockX = chunkIndexX * Constants::CHUNK_WIDTH + x;
				int32_t blockZ = chunkIndexZ * Constants::CHUNK_DEPTH + z;
				int32_t squareDistance = (blockX - worldCenterX) * (blockX - worldCenterX) + (blockZ - worldCenterZ) * (blockZ - worldCenterZ);

				float heightFactor = 1.0f;
				if (squareDistance < squareInnerRadius)
				{
					heightFactor = 1.0f;
				}
				else if (squareInnerRadius <= squareDistance && squareDistance <= squareOuterRadius)
				{
					heightFactor = (squareDistance - squareInnerRadius) * 1.0f / (squareOuterRadius - squareInnerRadius);
					heightFactor = 1.0f - heightFactor;
				}
				else
				{
					heightFactor = 0.0f;
				}

				float height = NoiseUtils::GetOctaveNoise
				(
					m_noiseEngine, 
					blockX * 1.0f,
					blockZ * 1.0f,
					m_worldGenParams.noiseNumOctaves,
					m_worldGenParams.noiseScale,
					m_worldGenParams.noisePersistence,
					m_worldGenParams.noiseLacunarity
				);
				height = (height + 1.0f) / 2.0f;
				height *= heightFactor;

				height = height * m_worldGenParams.worldMaxHeight;

				int ceilHeight = static_cast<int>(glm::ceil(height));
				for (int y = 0; y < ceilHeight; ++y)
				{
					Block* block = new Block(chunk->GetChunkIndices(), glm::ivec3(x, y, z));
					if (y < 5)
					{
						block->SetBlockType(BlockTypeEnum::STONE);
					}
					else if ((y > 8) && (y < 14))
					{
						block->SetBlockType(BlockTypeEnum::SAND);
					}
					else
					{
						block->SetBlockType(BlockTypeEnum::DIRT);
					}

					chunk->SetBlockAt(x, y, z, block);
				}

				int waterHeight = 10;
				for (int y = waterHeight; y >= 0; --y)
				{
					Block* block = chunk->GetBlockAt(x, y, z);
					if (block == nullptr)
					{
						block = new Block(chunk->GetChunkIndices(), glm::ivec3(x, y, z));
						block->SetBlockType(BlockTypeEnum::WATER);
						chunk->SetBlockAt(x, y, z, block);
					}
				}
			}
		}
		chunk->GenerateMesh();
		m_chunks.push_back(chunk);
	}

	return chunk;
}

/**
 * @brief Load chunks around the area defined by the center chunk index
 * and the radius in chunks
 * @param[in] centerChunkIndex Center chunk index
 * @param[in] radius Radius in chunks
 */
void World::LoadChunksWithinArea(const glm::ivec3& centerChunkIndex, const int& radius)
{
	for (int x = centerChunkIndex.x - radius; x <= centerChunkIndex.x + radius; ++x)
	{
		for (int z = centerChunkIndex.z - radius; z <= centerChunkIndex.z + radius; ++z)
		{
			if (GetChunkAt(x, z) == nullptr)
			{
				GenerateChunkAt(x, z);
			}
		}
	}
}

/**
 * @brief Unload chunks outside the area defined by the center chunk index
 * and the radius in chunks
 * @param[in] centerChunkIndex Center chunk index
 * @param radius Radius in chunks
 */
void World::UnloadChunksOutsideArea(const glm::ivec3& centerChunkIndex, const int& radius)
{
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		int chunkIndexX = m_chunks[i]->GetChunkIndexX();
		int chunkIndexZ = m_chunks[i]->GetChunkIndexZ();

		int minX = centerChunkIndex.x - radius;
		int maxX = centerChunkIndex.x + radius;

		int minZ = centerChunkIndex.z - radius;
		int maxZ = centerChunkIndex.z + radius;

		if ((chunkIndexX < minX) || (chunkIndexX > maxX) || (chunkIndexZ < minZ) || (chunkIndexZ > maxZ))
		{
			delete m_chunks[i];

			m_chunks[i] = m_chunks.back();
			m_chunks.pop_back();
			--i;
		}
	}
}

/**
 * @brief Draws the terrain meshes
 */
void World::DrawTerrainMeshes()
{
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		Mesh *terrainMesh = m_chunks[i]->GetTerrainMesh();
		terrainMesh->Draw();
	}
}

/**
 * @brief Draws the water meshes
 */
void World::DrawWaterMeshes()
{
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		Mesh *waterMesh = m_chunks[i]->GetWaterMesh();
		waterMesh->Draw();
	}
}

/**
 * @brief Casts a ray and gets the first non-air block hit
 * @param ray Ray
 * @param maxDistance Maximum distance
 * @return First non-air block hit by the ray cast. If no blocks are hit, returns nullptr.
 */
Block* World::Raycast(const Ray& ray, float maxDistance)
{
	float step = Constants::BLOCK_SIZE;
	for (float distance = 0.0f; distance <= maxDistance; distance += step)
	{
		glm::vec3 position = ray.origin + ray.direction * distance;
		Block* block = GetBlockAtWorldPosition(position);
		if (block != nullptr)
		{
			return block;
		}
	}

	return nullptr;
}
