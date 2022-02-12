#pragma once

#include "Graphics/Texture.hpp"

#include <map>
#include <string>

namespace Engine
{
/**
 * Manager class for handling loading/storing/unloading textures
 */
class TextureManager
{
private:
    /**
     * Map containing the mapping between the texture file path
     * and the texture data
     */
    std::map<std::string, Texture*> m_textures;

public:
    /**
     * @brief Constructor
     */
    TextureManager();

    /**
     * @brief Destructor
     */
    ~TextureManager();

    /**
     * @brief Loads the texture in the specified texture file path
     * @param[in] textureFilePath Texture file path
     * @return Flag indicating whether the operation was successful or not
     */
    bool LoadTexture(const std::string& textureFilePath);

    /**
     * @brief Gets the texture data of the texture at the specified file path
     * @param[in] textureFilePath Texture file path
     * @return Texture data
     */
    Texture* GetTexture(const std::string& textureFilePath);

    /**
     * @brief Unloads the texture data of the specified texture file path
     * @param[in] textureFilePath Texture file path
     */
    void UnloadTexture(const std::string& textureFilePath);

    /**
     * @brief Unloads all the textures that have been loaded
     */
    void UnloadAllTextures();
};
}
