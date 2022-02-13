#include "EntityTemplates/BlockTemplate.hpp"

/**
 * @brief Constructor
 */
BlockTemplate::BlockTemplate()
	: m_faceUVRects()
{
}

/**
 * @brief Copy constructor
 * @param[in] other Block template to copy from
 */
BlockTemplate::BlockTemplate(const BlockTemplate& other)
{
	m_faceUVRects = other.m_faceUVRects;
}

/**
 * Destructor
 */
BlockTemplate::~BlockTemplate()
{
}

/**
 * @brief Set the UV rect for the specified face
 * @param[in] face Face
 * @param[in] faceUVRect UV rect
 */
void BlockTemplate::SetFaceUVRect(const BlockFaceEnum& face, const glm::vec4& faceUVRect)
{
	m_faceUVRects[face] = faceUVRect;
}

/**
 * @brief Gets the UV rect for the specified face
 * @param[in] face Face
 * @return UV rect for the specified face
 */
const glm::vec4& BlockTemplate::GetFaceUVRect(const BlockFaceEnum& face) const
{
	return m_faceUVRects.find(face)->second;
}

/**
 * @brief Copy operator
 * @param[in] other Block template to copy from
 * @return Reference to the current instance
 */
BlockTemplate& BlockTemplate::operator=(const BlockTemplate& other)
{
	m_faceUVRects = other.m_faceUVRects;
	return *this;
}
