#pragma once

#include <glad/glad.h>

#include <string>

/**
 * Texture class
 */
class Texture
{
private:
	/**
	 * GL texture handle
	 */
	GLuint m_textureHandle;

	/**
	 * Texture width
	 */
	int m_width;

	/**
	 * Texture height
	 */
	int m_height;

public:
	/**
	 * @brief Constructor
	 */
	Texture();

	/**
	 * @brief Destructor
	 */
	~Texture();

	/**
	 * @brief Create a texture based on the iamge in the provided file path
	 * @param[in] textureFilePath Image file path
	 */
	void CreateFromFile(const std::string& textureFilePath);

	/**
	 * @brief Gets the GL texture handle for this texture
	 * @return GL texture handle
	 */
	GLuint GetHandle() const;

};
