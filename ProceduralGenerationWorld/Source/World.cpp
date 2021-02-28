#include "World.hpp"

#include "Constants.hpp"

/// <summary>
/// Constructor
/// </summary>
World::World()
	: m_chunks()
	, m_noiseEngine()
{
}

/// <summary>
/// Destructor
/// </summary>
World::~World()
{
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		delete m_chunks[i];
	}
	m_chunks.clear();
}

/// <summary>
/// Get chunk at the provided location indices
/// </summary>
/// <param name="chunkIndexX">Chunk x-index</param>
/// <param name="chunkIndexZ">Chunk z-index</param>
/// <returns>Chunk at the provided location. Returns nullptr if the chunk has not been generated yet.</returns>
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

/// <summary>
/// Gets the chunk at the given world position
/// </summary>
/// <param name="worldPosition">World position</param>
/// <returns>Chunk at the given world position</returns>
Chunk* World::GetChunkAtWorldPosition(const glm::vec3& worldPosition)
{
	glm::ivec3 chunkIndex = WorldPositionToChunkIndex(worldPosition);
	return GetChunkAt(chunkIndex.x, chunkIndex.z);
}

/// <summary>
/// Gets the block at the given world position
/// </summary>
/// <param name="worldPosition">World position</param>
/// <returns>Block at the given world position</returns>
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

/// <summary>
/// Converts the provided world position to chunk index
/// </summary>
/// <param name="worldPosition">World position</param>
/// <returns>Chunk index of the provided world position</returns>
glm::ivec3 World::WorldPositionToChunkIndex(const glm::vec3& worldPosition)
{
	glm::ivec3 ret(0);
	ret.x = static_cast<int>(glm::floor(worldPosition.x / Constants::BLOCK_SIZE / Constants::CHUNK_WIDTH));
	ret.z = static_cast<int>(glm::floor(worldPosition.z / Constants::BLOCK_SIZE / Constants::CHUNK_DEPTH));
	return ret;
}

/// <summary>
/// Generate chunk at the provided location indices
/// </summary>
/// <param name="chunkIndexX">Chunk x-index</param>
/// <param name="chunkIndexZ">Chunk z-index</param>
/// <returns>Generated chunk</returns>
Chunk* World::GenerateChunkAt(const int& chunkIndexX, const int& chunkIndexZ)
{
	Chunk* chunk = new Chunk(chunkIndexX, chunkIndexZ);
	for (int x = 0; x < Constants::CHUNK_WIDTH; ++x)
	{
		for (int z = 0; z < Constants::CHUNK_DEPTH; ++z)
		{
			float height = m_noiseEngine.GetNoise((chunkIndexX * Constants::CHUNK_WIDTH + x) * 1.0f, (chunkIndexZ * Constants::CHUNK_DEPTH + z) * 1.0f);
			height = (height + 1.0f) / 2.0f;
			height = height * 10.0f;

			int ceilHeight = static_cast<int>(glm::ceil(height));
			for (int y = 0; y < ceilHeight; ++y)
			{
				Block* block = new Block(chunk->GetChunkIndices(), glm::ivec3(x, y, z));
				block->SetBlockType(BlockType::Dirt);
				chunk->SetBlockAt(x, y, z, block);
			}
		}
	}
	chunk->GenerateMesh();
	m_chunks.push_back(chunk);

	return chunk;
}

/// <summary>
/// Draws the world
/// </summary>
void World::Draw()
{
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		m_chunks[i]->Draw();
	}
}

/// <summary>
/// Casts a ray and gets the first non-air block hit
/// </summary>
/// <param name="ray">Ray</param>
/// <param name="maxDistance">Maximum distance</param>
/// <returns>First non-air block hit by the ray cast. If no blocks are hit, returns nullptr.</returns>
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
