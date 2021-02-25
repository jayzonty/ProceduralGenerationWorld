#pragma once

/// <summary>
/// Block type enum
/// </summary>
enum class BlockType
{
	Air,
	Dirt
};

/// <summary>
/// Block class
/// </summary>
struct Block
{
private:
	/// <summary>
	/// Block type
	/// </summary>
	BlockType m_type;

public:

	/// <summary>
	/// Constructor
	/// </summary>
	Block();

	/// <summary>
	/// Destructor
	/// </summary>
	~Block();

	/// <summary>
	/// Sets the block type
	/// </summary>
	/// <param name="type">New block type</param>
	void SetBlockType(BlockType type);

	/// <summary>
	/// Gets this block's type
	/// </summary>
	/// <returns></returns>
	BlockType GetBlockType() const;

};
