#pragma once

#include <FastNoiseLite/FastNoiseLite.h>

#include "Block.hpp"
#include "Mesh.hpp"

/// <summary>
/// Chunk class
/// </summary>
class Chunk
{
private:
	/// <summary>
	/// Chunk width constant
	/// </summary>
	const int CHUNK_WIDTH = 16;

	/// <summary>
	/// Chunk height constant
	/// </summary>
	const int CHUNK_HEIGHT = 256;

	/// <summary>
	/// Chunk depth constant
	/// </summary>
	const int CHUNK_DEPTH = 16;

	/// <summary>
	/// Chunk mesh
	/// </summary>
	Mesh m_mesh;

	/// <summary>
	/// Block data
	/// </summary>
	std::vector<Block> m_blocks;

	/// <summary>
	/// Chunk x-index
	/// </summary>
	int m_chunkIndexX;

	/// <summary>
	/// Chunk z-index
	/// </summary>
	int m_chunkIndexZ;

public:

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="chunkIndexX">Chunk x-index</param>
	/// <param name="chunkIndexZ">Chunk z-index</param>
	Chunk(const int& chunkIndexX, const int& chunkIndexZ);

	/// <summary>
	/// Destructor
	/// </summary>
	~Chunk();

	/// <summary>
	/// Gets the x chunk index for this chunk
	/// </summary>
	/// <returns></returns>
	int GetChunkIndexX() const;

	/// <summary>
	/// Gets the z chunk index for this chunk
	/// </summary>
	/// <returns></returns>
	int GetChunkIndexZ() const;

	/// <summary>
	/// Generates the chunk
	/// </summary>
	void GenerateChunk(FastNoiseLite& noise);

	/// <summary>
	/// Draw the chunk
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// Gets the block at the specified location
	/// </summary>
	/// <param name="x">X-coordinate</param>
	/// <param name="y">Y-coordinate</param>
	/// <param name="z">Z-coordinate</param>
	/// <returns></returns>
	Block& GetBlockAt(size_t x, size_t y, size_t z);

};
