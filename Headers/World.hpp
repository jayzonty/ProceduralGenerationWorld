#pragma once

#include "Camera.hpp"
#include "Chunk.hpp"
#include "Ray.hpp"

#include <FastNoiseLite/FastNoiseLite.h>

#include <vector>

/**
 * World data class
 */
class World
{
private:
	/**
	 * List of chunks
	 */
	std::vector<Chunk*> m_chunks;

	/**
	 * Noise generator
	 */
	FastNoiseLite m_noiseEngine;

public:
	/**
	 * @brief Constructor
	 */
	World();

	/**
	 * @brief Destructor
	 */
	~World();

	/**
	 * @brief Get chunk at the provided location indices
	 * @param[in] chunkIndexX Chunk x-index
	 * @param[in] chunkIndexZ Chunk z-index
	 * @return Chunk at the provided location. Returns nullptr if the chunk has not been generated yet
	 */
	Chunk* GetChunkAt(const int& chunkIndexX, const int& chunkIndexZ);

	/**
	 * @brief Gets the chunk at the given world position
	 * @param[in] worldPosition World position
	 * @return Chunk at the given world position
	 */
	Chunk* GetChunkAtWorldPosition(const glm::vec3& worldPosition);

	/**
	 * @brief Gets the block at the given world position
	 * @param[in] worldPosition World position
	 * @return Block at the given world position
	 */
	Block* GetBlockAtWorldPosition(const glm::vec3& worldPosition);

	/**
	 * @brief Converts the provided world position to chunk index
	 * @param[in] worldPosition World position
	 * @return Chunk index of the provided world position
	 */
	glm::ivec3 WorldPositionToChunkIndex(const glm::vec3& worldPosition);

	/**
	 * @brief Generate chunk at the provided location indices
	 * @param[in] chunkIndexX Chunk x-index
	 * @param[in] chunkIndexZ Chunk z-index
	 * @return Generated chunk
	 */
	Chunk* GenerateChunkAt(const int& chunkIndexX, const int& chunkIndexZ);

	/**
	 * @brief Load chunks around the area defined by the center chunk index
	 * and the radius in chunks
	 * @param[in] centerChunkIndex Center chunk index
	 * @param[in] radius Radius in chunks
	 */
	void LoadChunksWithinArea(const glm::ivec3& centerChunkIndex, const int& radius);

	/**
	 * @brief Unload chunks outside the area defined by the center chunk index
	 * and the radius in chunks
	 * @param[in] centerChunkIndex Center chunk index
	 * @param[in] radius Radius in chunks
	 */
	void UnloadChunksOutsideArea(const glm::ivec3& centerChunkIndex, const int& radius);

	/**
	 * @brief Draws the world
	 * @param[in] camera Camera
	 */
	void Draw(const Camera& camera);

	/**
	 * @brief Casts a ray and gets the first non-air block hit
	 * @param[in] ray Ray
	 * @param[in] maxDistance Maximum distance
	 * @return First non-air block hit by the ray cast. If no blocks are hit, returns nullptr.
	 */
	Block* Raycast(const Ray& ray, float maxDistance);
};
