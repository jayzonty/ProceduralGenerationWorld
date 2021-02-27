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
	/// Chunk mesh
	/// </summary>
	Mesh m_mesh;

	/// <summary>
	/// Block data
	/// </summary>
	std::vector<Block*> m_blocks;

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
	/// Generates the mesh for this chunk
	/// </summary>
	void GenerateMesh();

	/// <summary>
	/// Draw the chunk
	/// </summary>
	void Draw();

	/// <summary>
	/// Gets the block at the specified location
	/// </summary>
	/// <param name="x">X-coordinate</param>
	/// <param name="y">Y-coordinate</param>
	/// <param name="z">Z-coordinate</param>
	/// <returns></returns>
	Block* GetBlockAt(int x, int y, int z);

};
