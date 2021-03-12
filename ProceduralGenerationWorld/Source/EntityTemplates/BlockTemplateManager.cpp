#include "EntityTemplates/BlockTemplateManager.hpp"

/// <summary>
/// Constructor
/// </summary>
BlockTemplateManager::BlockTemplateManager()
	: m_templates()
{
}

/// <summary>
/// Destructor
/// </summary>
BlockTemplateManager::~BlockTemplateManager()
{
}

/// <summary>
/// Gets the singleton instance for the BlockTemplateManager
/// </summary>
/// <returns>Singleton instance of this class</returns>
BlockTemplateManager& BlockTemplateManager::GetInstance()
{
	static BlockTemplateManager instance;
	return instance;
}

/// <summary>
	/// Add the block template for the specified block type
	/// </summary>
	/// <param name="blockType">Block type</param>
	/// <param name="blockTemplate">Block template</param>
void BlockTemplateManager::AddBlockTemplate(const BlockTypeEnum& blockType, const BlockTemplate& blockTemplate)
{
	m_templates[blockType] = blockTemplate;
}

/// <summary>
/// Gets the block template for the specified block type
/// </summary>
/// <returns>Block template for the specified block type. Returns nullptr if the block type does not have a template</returns>
const BlockTemplate* BlockTemplateManager::GetBlockTemplate(const BlockTypeEnum& blockType)
{
	if (m_templates.find(blockType) != m_templates.end())
	{
		return &m_templates[blockType];
	}

	return nullptr;
}
