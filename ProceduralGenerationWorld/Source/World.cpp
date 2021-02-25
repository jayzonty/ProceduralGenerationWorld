#include "World.hpp"

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
/// Generate chunk at the provided location indices
/// </summary>
/// <param name="chunkIndexX">Chunk x-index</param>
/// <param name="chunkIndexZ">Chunk z-index</param>
/// <returns>Generated chunk</returns>
Chunk* World::GenerateChunkAt(const int& chunkIndexX, const int& chunkIndexZ)
{
	Chunk* chunk = new Chunk(chunkIndexX, chunkIndexZ);
	for (int x = 0; x < Chunk::CHUNK_WIDTH; ++x)
	{
		for (int z = 0; z < Chunk::CHUNK_DEPTH; ++z)
		{
			float height = m_noiseEngine.GetNoise((chunkIndexX * Chunk::CHUNK_WIDTH + x) * 1.0f, (chunkIndexZ * Chunk::CHUNK_DEPTH + z) * 1.0f);
			height = (height + 1.0f) / 2.0f;
			height = height * 10.0f;

			size_t ceilHeight = static_cast<size_t>(glm::ceil(height));
			for (size_t y = 0; y < ceilHeight; ++y)
			{
				chunk->GetBlockAt(x, y, z).SetBlockType(BlockType::Dirt);
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
