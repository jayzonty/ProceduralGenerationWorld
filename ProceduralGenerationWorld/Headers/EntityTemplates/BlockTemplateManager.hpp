#pragma once

#include <Enums/BlockTypeEnum.hpp>

#include "BlockTemplate.hpp"

#include <map>

class BlockTemplateManager
{
private:
	/// <summary>
	/// Mapping of block type and its corresponding block template
	/// </summary>
	std::map<BlockTypeEnum, BlockTemplate> m_templates;

	/// <summary>
	/// Constructor
	/// </summary>
	BlockTemplateManager();

	/* Delete copy constructor */
	BlockTemplateManager(const BlockTemplateManager&) = delete;

	/* Delete copy operator */
	BlockTemplateManager& operator=(const BlockTemplateManager&) = delete;

public:
	/// <summary>
	/// Destructor
	/// </summary>
	~BlockTemplateManager();

	/// <summary>
	/// Gets the singleton instance for the BlockTemplateManager
	/// </summary>
	/// <returns>Singleton instance of this class</returns>
	static BlockTemplateManager& GetInstance();

	/// <summary>
	/// Add the block template for the specified block type
	/// </summary>
	/// <param name="blockType">Block type</param>
	/// <param name="blockTemplate">Block template</param>
	void AddBlockTemplate(const BlockTypeEnum& blockType, const BlockTemplate& blockTemplate);

	/// <summary>
	/// Gets the block template for the specified block type
	/// </summary>
	/// <returns>Block template for the specified block type. Returns nullptr if the block type does not have a template</returns>
	const BlockTemplate* GetBlockTemplate(const BlockTypeEnum& blockType);

};
