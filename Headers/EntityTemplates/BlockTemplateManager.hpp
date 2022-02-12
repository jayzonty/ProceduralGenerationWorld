#pragma once

#include "Enums/BlockTypeEnum.hpp"

#include "BlockTemplate.hpp"

#include <map>

/**
 * Manager class for block templates
 */
class BlockTemplateManager
{
private:
	/**
	 * Mapping of block type and its corresponding block template
	 */
	std::map<BlockTypeEnum, BlockTemplate> m_templates;

	/**
	 * @brief Constructor
	 */
	BlockTemplateManager();

	/* Delete copy constructor */
	BlockTemplateManager(const BlockTemplateManager&) = delete;

	/* Delete copy operator */
	BlockTemplateManager& operator=(const BlockTemplateManager&) = delete;

public:
	/**
	 * @brief Destructor
	 */
	~BlockTemplateManager();

	/**
	 * @brief Gets the singleton instance for the BlockTemplateManager
	 * @return Singleton instance of this class
	 */
	static BlockTemplateManager& GetInstance();

	/**
	 * @brief Add the block template for the specified block type
	 * @param[in] blockType Block type
	 * @param[in] blockTemplate Block template
	 */
	void AddBlockTemplate(const BlockTypeEnum& blockType, const BlockTemplate& blockTemplate);

	/**
	 * @brief Gets the block template for the specified block type
	 * @return Block template for the specified block type. Returns nullptr if the block type does not have a template
	 */
	const BlockTemplate* GetBlockTemplate(const BlockTypeEnum& blockType);
};
