#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/// <summary>
/// Constructor
/// </summary>
Texture::Texture()
	: m_textureHandle(0)
	, m_width(0)
	, m_height(0)
{
}

/// <summary>
/// Destructor
/// </summary>
Texture::~Texture()
{
	if (m_textureHandle != 0)
	{
		glDeleteTextures(1, &m_textureHandle);
		m_textureHandle = 0;
	}
}

/// <summary>
/// Create a texture based on the iamge in the provided file path
/// </summary>
/// <param name="textureFilePath">Image file path</param>
void Texture::CreateFromFile(const std::string& textureFilePath)
{
	if (m_textureHandle == 0)
	{
		glGenTextures(1, &m_textureHandle);
	}

	stbi_set_flip_vertically_on_load(true);

	int numChannels;
	unsigned char* data = stbi_load(textureFilePath.c_str(), &m_width, &m_height, &numChannels, 0);

	GLenum imageDataFormat = GL_RGB;
	if (numChannels == 4)
	{
		imageDataFormat = GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D, m_textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, imageDataFormat, m_width, m_height, 0, imageDataFormat, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

/// <summary>
/// Gets the GL texture handle for this texture
/// </summary>
/// <returns>GL texture handle</returns>
GLuint Texture::GetHandle() const
{
	return m_textureHandle;
}
