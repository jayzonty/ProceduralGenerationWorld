#include "Block.hpp"

#include "Constants.hpp"

/**
 * @brief Constructor
 * @param[in] chunkIndex Chunk index of the chunk this block belongs to
 * @param[in] positionInChunk Position of this block within the chunk
 */
Block::Block(const glm::ivec3& chunkIndex, const glm::ivec3& positionInChunk)
	: m_type(BlockTypeEnum::AIR)
	, m_chunkIndex(chunkIndex)
	, m_positionInChunk(positionInChunk)
{
}

/**
 * @brief Destructor
 */
Block::~Block()
{
}

/**
 * @brief Sets the block type
 * @param[in] type New block type
 */
void Block::SetBlockType(BlockTypeEnum type)
{
	m_type = type;
}

/**
 * @brief Gets this block's type
 * @return Block type
 */
BlockTypeEnum Block::GetBlockType() const
{
	return m_type;
}

/**
 * @brief Gets the position of this block within the chunk
 * @return Position of this block within the chunk
 */
glm::ivec3 Block::GetPositionInChunk() const
{
	return m_positionInChunk;
}

/**
 * @brief Gets the position of this block in the world
 * @return Position of this block in the world
 */
glm::ivec3 Block::GetPositionInWorld() const
{
	glm::ivec3 ret;
	ret.x = m_chunkIndex.x * Constants::CHUNK_WIDTH + m_positionInChunk.x;
	ret.y = m_chunkIndex.y * Constants::CHUNK_HEIGHT + m_positionInChunk.y;
	ret.z = m_chunkIndex.z * Constants::CHUNK_DEPTH + m_positionInChunk.z;

	return ret;
}
