#pragma once

#include "Enums/BlockFaceEnum.hpp"

#include <glm/glm.hpp>

#include <map>

/**
 * Block template class
 */
class BlockTemplate
{
private:
	/**
	 * Mapping between the block face and its corresponding UV rect
	 */
	std::map<BlockFaceEnum, glm::vec4> m_faceUVRects;

public:
	/**
	 * @brief Constructor
	 */
	BlockTemplate();

	/**
	 * Copy constructor
	 * @param[in] other Block template to copy from
	 */
	BlockTemplate(const BlockTemplate& other);

	/**
	 * @brief Destructor
	 */
	~BlockTemplate();

	/**
	 * @brief Set the UV rect for the specified face
	 * @param[in] face Face
	 * @param[in] faceUVRect UV Rect
	 */
	void SetFaceUVRect(const BlockFaceEnum& face, const glm::vec4& faceUVRect);

	/**
	 * @brief Gets the UV rect for the specified face
	 * @param[in] face Face
	 * @return UV rect for the specified face
	 */
	const glm::vec4& GetFaceUVRect(const BlockFaceEnum& face) const;

	/**
	 * @brief Copy operator
	 * @param[in] other Block template to copy from
	 * @return Reference to the current instance
	 */
	BlockTemplate& operator=(const BlockTemplate& other);
};
