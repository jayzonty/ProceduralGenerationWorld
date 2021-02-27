#include "Block.hpp"

#include "Constants.hpp"

/// <summary>
/// Constructor
/// </summary>
Block::Block(const glm::ivec3& chunkIndex, const glm::ivec3& positionInChunk)
	: m_type(BlockType::Air)
	, m_chunkIndex(chunkIndex)
	, m_positionInChunk(positionInChunk)
{
}

/// <summary>
/// Destructor
/// </summary>
Block::~Block()
{
}

/// <summary>
/// Sets the block type
/// </summary>
/// <param name="type">New block type</param>
void Block::SetBlockType(BlockType type)
{
	m_type = type;
}

/// <summary>
/// Gets this block's type
/// </summary>
/// <returns></returns>
BlockType Block::GetBlockType() const
{
	return m_type;
}

/// <summary>
/// Gets the position of this block within the chunk
/// </summary>
/// <returns>Position of this block within the chunk</returns>
glm::ivec3 Block::GetPositionInChunk() const
{
	return m_positionInChunk;
}

/// <summary>
/// Gets the position of this block in the world
/// </summary>
/// <returns>Position of this block in the world</returns>
glm::ivec3 Block::GetPositionInWorld() const
{
	glm::ivec3 ret;
	ret.x = m_chunkIndex.x * Constants::CHUNK_WIDTH + m_positionInChunk.x;
	ret.y = m_chunkIndex.y * Constants::CHUNK_HEIGHT + m_positionInChunk.y;
	ret.z = m_chunkIndex.z * Constants::CHUNK_DEPTH + m_positionInChunk.z;

	return ret;
}
