#pragma once

#include "Chunk.hpp"

#include <FastNoiseLite/FastNoiseLite.h>

#include <vector>

/// <summary>
/// World data class
/// </summary>
class World
{
private:
	/// <summary>
	/// List of chunks
	/// </summary>
	std::vector<Chunk*> m_chunks;

	/// <summary>
	/// Noise generator
	/// </summary>
	FastNoiseLite m_noiseEngine;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	World();

	/// <summary>
	/// Destructor
	/// </summary>
	~World();

	/// <summary>
	/// Get chunk at the provided location indices
	/// </summary>
	/// <param name="chunkIndexX">Chunk x-index</param>
	/// <param name="chunkIndexZ">Chunk z-index</param>
	/// <returns>Chunk at the provided location. Returns nullptr if the chunk has not been generated yet.</returns>
	Chunk* GetChunkAt(const int& chunkIndexX, const int& chunkIndexZ);

	/// <summary>
	/// Generate chunk at the provided location indices
	/// </summary>
	/// <param name="chunkIndexX">Chunk x-index</param>
	/// <param name="chunkIndexZ">Chunk z-index</param>
	/// <returns>Generated chunk</returns>
	Chunk* GenerateChunkAt(const int& chunkIndexX, const int& chunkIndexZ);

	/// <summary>
	/// Draws the world
	/// </summary>
	void Draw();
};
