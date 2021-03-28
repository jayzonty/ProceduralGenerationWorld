#include "EntityTemplates/BlockTemplate.hpp"

/// <summary>
/// Constructor
/// </summary>
BlockTemplate::BlockTemplate()
	: m_faceUVRects()
{
}

/// <summary>
/// Copy constructor
/// </summary>
/// <param name="other">Block template to copy from</param>
BlockTemplate::BlockTemplate(const BlockTemplate& other)
{
	m_faceUVRects = other.m_faceUVRects;
}

/// <summary>
/// Destructor
/// </summary>
BlockTemplate::~BlockTemplate()
{
}

/// <summary>
/// Set the UV rect for the specified face
/// </summary>
/// <param name="face">Face</param>
/// <param name="faceUVRect">UV Rect</param>
void BlockTemplate::SetFaceUVRect(const BlockFaceEnum& face, const glm::vec4& faceUVRect)
{
	m_faceUVRects[face] = faceUVRect;
}

/// <summary>
/// Gets the UV rect for the specified face
/// </summary>
/// <param name="face">Face</param>
/// <returns>UV rect for the specified face</returns>
const glm::vec4& BlockTemplate::GetFaceUVRect(const BlockFaceEnum& face) const
{
	return m_faceUVRects.find(face)->second;
}

/// <summary>
/// Copy operator
/// </summary>
/// <param name="other">Block template to copy from</param>
/// <returns>Reference to the current instance</returns>
BlockTemplate& BlockTemplate::operator=(const BlockTemplate& other)
{
	m_faceUVRects = other.m_faceUVRects;
	return *this;
}
