#include "EntityTemplates/BlockTemplateManager.hpp"

/**
 * @brief Constructor
 */
BlockTemplateManager::BlockTemplateManager()
	: m_templates()
{
}

/**
 * @brief Destructor
 */
BlockTemplateManager::~BlockTemplateManager()
{
}

/**
 * @brief Gets the singleton instance for the BlockTemplateManager
 * @return Singleton instance of this class
 */
BlockTemplateManager& BlockTemplateManager::GetInstance()
{
	static BlockTemplateManager instance;
	return instance;
}

/**
 * @brief Add the block template for the specified block type
 * @param[in] blockType Block type
 * @param[in] blockTemplate Block template
 */
void BlockTemplateManager::AddBlockTemplate(const BlockTypeEnum& blockType, const BlockTemplate& blockTemplate)
{
	m_templates[blockType] = blockTemplate;
}

/**
 * @brief Gets the block template for the specified block type
 * @return Block template for the specified block type. Returns nullptr if the block type does not have a template
 */
const BlockTemplate* BlockTemplateManager::GetBlockTemplate(const BlockTypeEnum& blockType)
{
	if (m_templates.find(blockType) != m_templates.end())
	{
		return &m_templates[blockType];
	}

	return nullptr;
}
