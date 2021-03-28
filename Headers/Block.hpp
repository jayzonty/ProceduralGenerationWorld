#pragma once

#include "Enums/BlockTypeEnum.hpp"

#include <glm/glm.hpp>

/// <summary>
/// Block class
/// </summary>
struct Block
{
private:
	/// <summary>
	/// Block type
	/// </summary>
	BlockTypeEnum m_type;

	/// <summary>
	/// Chunk index of the chunk this block belongs to
	/// </summary>
	glm::ivec3 m_chunkIndex;

	/// <summary>
	/// Position of this block within the chunk
	/// </summary>
	glm::ivec3 m_positionInChunk;

public:

	/// <summary>
	/// Constructor
	/// <param name="chunkIndex">Chunk index of the chunk this block belongs to</param>
	/// <param name="positionInChunk">Position of this block within the chunk</param>
	/// </summary>
	Block(const glm::ivec3& chunkIndex, const glm::ivec3& positionInChunk);

	/// <summary>
	/// Destructor
	/// </summary>
	~Block();

	/// <summary>
	/// Sets the block type
	/// </summary>
	/// <param name="type">New block type</param>
	void SetBlockType(BlockTypeEnum type);

	/// <summary>
	/// Gets this block's type
	/// </summary>
	/// <returns></returns>
	BlockTypeEnum GetBlockType() const;

	/// <summary>
	/// Gets the position of this block within the chunk
	/// </summary>
	/// <returns>Position of this block within the chunk</returns>
	glm::ivec3 GetPositionInChunk() const;

	/// <summary>
	/// Gets the position of this block in the world
	/// </summary>
	/// <returns>Position of this block in the world</returns>
	glm::ivec3 GetPositionInWorld() const;

};
