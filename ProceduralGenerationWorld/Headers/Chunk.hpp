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
	/// Chunk indices in each axis
	/// </summary>
	glm::ivec3 m_chunkIndex;

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
	/// <returns>Chunk x-index</returns>
	int GetChunkIndexX() const;

	/// <summary>
	/// Gets the y chunk index for this chunk
	/// </summary>
	/// <returns>Chunk y-index</returns>
	int GetChunkIndexY() const;

	/// <summary>
	/// Gets the z chunk index for this chunk
	/// </summary>
	/// <returns>Chunk z-index</returns>
	int GetChunkIndexZ() const;

	/// <summary>
	/// Gets the chunk indices in each axis for this chunk
	/// </summary>
	/// <returns>Chunk indices for each axis</returns>
	glm::ivec3 GetChunkIndices() const;

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

	/// <summary>
	/// Sets the block at the specified location with the specified block
	/// </summary>
	/// <param name="x">X-coordinate</param>
	/// <param name="y">Y-coordinate</param>
	/// <param name="z">Z-coordinate</param>
	/// <param name="block">Data for the new block. Can be set to nullptr if it's an air block.</param>
	void SetBlockAt(const int& x, const int& y, const int& z, Block* block);

};
