#include "Block.hpp"

/// <summary>
/// Constructor
/// </summary>
Block::Block()
	: m_type(BlockType::Air)
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
