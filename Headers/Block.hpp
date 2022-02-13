#pragma once

#include "Enums/BlockTypeEnum.hpp"

#include <glm/glm.hpp>

/**
 * Block class
 */
struct Block
{
private:
	/**
	 * Block type
	 */
	BlockTypeEnum m_type;

	/**
	 * Chunk index of the chunk this block belongs to
	 */
	glm::ivec3 m_chunkIndex;

	/**
	 * Position of this block within the chunk
	 */
	glm::ivec3 m_positionInChunk;

public:
	/**
	 * @brief Constructor
	 * @param[in] chunkIndex Chunk index of the chunk this block belongs to
	 * @param[in] positionInChunk Position of this block within the chunk
	 */
	Block(const glm::ivec3& chunkIndex, const glm::ivec3& positionInChunk);

	/**
	 * @brief Destructor
	 */
	~Block();

	/**
	 * @brief Sets the block type
	 * @param[in] type New block type
	 */
	void SetBlockType(BlockTypeEnum type);

	/**
	 * @brief Gets this block's type
	 * @return Block type
	 */
	BlockTypeEnum GetBlockType() const;

	/**
	 * @brief Gets the position of this block within the chunk
	 * @return Position of this block within the chunk
	 */
	glm::ivec3 GetPositionInChunk() const;

	/**
	 * @brief Gets the position of this block in the world
	 * @return Position of this block in the world
	 */
	glm::ivec3 GetPositionInWorld() const;

};
