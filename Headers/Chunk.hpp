#pragma once

#include <FastNoiseLite/FastNoiseLite.h>

#include "Block.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"

/**
 * Chunk class
 */
class Chunk
{
private:
	/**
	 * Chunk mesh
	 */
	Mesh m_mesh;

	/**
	 * Water mesh
	 */
	Mesh m_waterMesh;

	/**
	 * Block data
	 */
	std::vector<Block*> m_blocks;

	/**
	 * Chunk indices in each axis
	 */
	glm::ivec3 m_chunkIndex;

public:
	/**
	 * @brief Constructor
	 * @param[in] chunkIndexX Chunk x-index
	 * @param[in] chunkIndexZ Chunk z-index
	 */
	Chunk(const int& chunkIndexX, const int& chunkIndexZ);

	/**
	 * @brief Destructor
	 */
	~Chunk();

	/**
	 * @brief Gets the x chunk index for this chunk
	 * @return Chunk x-index
	 */
	int GetChunkIndexX() const;

	/**
	 * @brief Gets the y chunk index for this chunk
	 * @return Chunk y-index
	 */
	int GetChunkIndexY() const;

	/**
	 * @brief Gets the z chunk index for this chunk
	 * @return Chunk z-index
	 */
	int GetChunkIndexZ() const;

	/**
	 * @brief Gets the chunk indices in each axis for this chunk
	 * @return Chunk indices for each axis
	 */
	glm::ivec3 GetChunkIndices() const;

	/**
	 * @brief Generates the mesh for this chunk
	 */
	void GenerateMesh();

	/**
	 * @brief Draw the chunk
	 * @param[in] camera Camera
	 */
	void Draw(const Camera& camera);

	/**
	 * @brief Gets the block at the specified location
	 * @param[in] x X-coordinate
	 * @param[in] y Y-coordinate
	 * @param[in] z z-coordinate
	 * @return Block data of the block at the specified location
	 */
	Block* GetBlockAt(int x, int y, int z);

	/**
	 * @brief Sets the block at the specified location with the specified block
	 * @param[in] x X-coordinate
	 * @param[in] y Y-coordinate
	 * @param[in] z z-coordinate
	 * @param[in] block Data for the new block. Can be set to nullptr if it's an air block.
	 */
	void SetBlockAt(const int& x, const int& y, const int& z, Block* block);
};
