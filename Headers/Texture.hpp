#pragma once

#include "Image.hpp"

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

	/**
	 * Image format
	 */
	GLenum m_imageFormat;

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
	 * @brief Creates a texture from the specified image data
	 * @param[in] image Image data
	 */
	void CreateFromImage(const Image &image);

	/**
	 * @brief Updates the texture data from the specified image data
	 * @param[in] image Image data
	 */
	void UpdateFromImageData(const Image &image);

	/**
	 * @brief Gets the GL texture handle for this texture
	 * @return GL texture handle
	 */
	GLuint GetHandle() const;

};
