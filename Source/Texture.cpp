#include "Texture.hpp"
#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/**
 * @brief Constructor
 */
Texture::Texture()
	: m_textureHandle(0)
	, m_width(0)
	, m_height(0)
{
}

/**
 * @brief Destructor
 */
Texture::~Texture()
{
	if (m_textureHandle != 0)
	{
		glDeleteTextures(1, &m_textureHandle);
		m_textureHandle = 0;
	}
}

/**
 * @brief Create a texture based on the iamge in the provided file path
 * @param[in] textureFilePath Image file path
 */
void Texture::CreateFromFile(const std::string& textureFilePath)
{
	Image image = Image::LoadFromFile(textureFilePath);
	CreateFromImage(image);
}

/**
 * @brief Creates a texture from the specified image data
 * @param[in] image Image data
 */
void Texture::CreateFromImage(const Image &image)
{
	m_width = image.width;
	m_height = image.height;

	if (m_textureHandle == 0)
	{
		glGenTextures(1, &m_textureHandle);
	}

	GLenum imageDataFormat = GL_RGB;
	if (image.numChannels == 4)
	{
		imageDataFormat = GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D, m_textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D
	(
		GL_TEXTURE_2D, 
		0, 
		imageDataFormat, 
		image.width,
		image.height,
		0,
		imageDataFormat,
		GL_UNSIGNED_BYTE, 
		image.data.data()
	);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * @brief Gets the GL texture handle for this texture
 * @return GL texture handle
 */
GLuint Texture::GetHandle() const
{
	return m_textureHandle;
}
