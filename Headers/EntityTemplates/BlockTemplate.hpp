#pragma once

#include <Enums/BlockFaceEnum.hpp>

#include <glm/glm.hpp>

#include <map>

/// <summary>
/// Block template class
/// </summary>
class BlockTemplate
{
private:
	/// <summary>
	/// Mapping between the block face and its corresponding UV rect
	/// </summary>
	std::map<BlockFaceEnum, glm::vec4> m_faceUVRects;

public:

	/// <summary>
	/// Constructor
	/// </summary>
	BlockTemplate();

	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="other">Block template to copy from</param>
	BlockTemplate(const BlockTemplate& other);

	/// <summary>
	/// Destructor
	/// </summary>
	~BlockTemplate();

	/// <summary>
	/// Set the UV rect for the specified face
	/// </summary>
	/// <param name="face">Face</param>
	/// <param name="faceUVRect">UV Rect</param>
	void SetFaceUVRect(const BlockFaceEnum& face, const glm::vec4& faceUVRect);

	/// <summary>
	/// Gets the UV rect for the specified face
	/// </summary>
	/// <param name="face">Face</param>
	/// <returns>UV rect for the specified face</returns>
	const glm::vec4& GetFaceUVRect(const BlockFaceEnum& face) const;

	/// <summary>
	/// Copy operator
	/// </summary>
	/// <param name="other">Block template to copy from</param>
	/// <returns>Reference to the current instance</returns>
	BlockTemplate& operator=(const BlockTemplate& other);

};
