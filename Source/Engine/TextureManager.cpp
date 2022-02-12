#include "Engine/TextureManager.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

namespace Engine
{
/**
 * @brief Constructor
 */
TextureManager::TextureManager()
    : m_textures()
{
}

/**
 * @brief Destructor
 */
TextureManager::~TextureManager()
{
    UnloadAllTextures();
}

/**
 * @brief Loads the texture in the specified texture file path
 * @param[in] textureFilePath Texture file path
 * @return Flag indicating whether the operation was successful or not
 */
bool TextureManager::LoadTexture(const std::string &textureFilePath)
{
    if (m_textures.find(textureFilePath) != m_textures.end())
    {
        return true;
    }

    Texture *texture = new Texture();
    m_textures[textureFilePath] = texture;

    glGenTextures(1, &texture->texID);

    stbi_set_flip_vertically_on_load(true);

    int numChannels;
    unsigned char* data = stbi_load(textureFilePath.c_str(), &texture->width, &texture->height, &numChannels, 0);
    if (data == nullptr)
    {
        return false;
    }

	GLenum imageDataFormat = GL_RGB;
	if (numChannels == 4)
	{
		imageDataFormat = GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D, texture->texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, imageDataFormat, texture->width, texture->height, 0, imageDataFormat, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return true;
}

/**
 * @brief Gets the texture data of the texture at the specified file path
 * @param[in] textureFilePath Texture file path
 * @return Texture data
 */
Texture* TextureManager::GetTexture(const std::string &textureFilePath)
{
    if ((m_textures.find(textureFilePath) != m_textures.end())
        || LoadTexture(textureFilePath))
    {
        return m_textures[textureFilePath];
    }

    return nullptr;
}

/**
 * @brief Unloads the texture data of the specified texture file path
 * @param[in] textureFilePath Texture file path
 */
void TextureManager::UnloadTexture(const std::string &textureFilePath)
{
    if (m_textures.find(textureFilePath) == m_textures.end())
    {
        return;
    }

    Texture *texture = m_textures[textureFilePath];
    m_textures.erase(textureFilePath);

    glDeleteTextures(1, &texture->texID);
    delete texture;
}

/**
 * @brief Unloads all the textures that have been loaded
 */
void TextureManager::UnloadAllTextures()
{
    for (auto& it : m_textures)
    {
        delete it.second;
    }
    m_textures.clear();
}
}
