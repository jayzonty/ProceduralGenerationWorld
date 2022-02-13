#pragma once

#include "Engine/Graphics/ShaderProgram.hpp"
#include "Engine/Graphics/Texture.hpp"
#include "Engine/TextureManager.hpp"

#include <map>
#include <string>

/**
 * Resource manager class
 */
class ResourceManager
{
private:
	/**
	 * List of shaders identified by their corresponding string keys
	 */
	std::map<std::string, Engine::ShaderProgram*> m_shaders;

	/**
	 * List of textures identified by their corresponding string keys
	 */
	std::map<std::string, Engine::Texture*> m_textures;

	/**
	 * Texture manager
	 */
	Engine::TextureManager m_textureManager;

	/**
	 * @brief Constructor
	 */
	ResourceManager();

	/* Delete copy constructor */
	ResourceManager(const ResourceManager&) = delete;

	/* Delete copy operator */
	ResourceManager& operator=(const ResourceManager&) = delete;

public:
	/**
	 * @brief Destructor
	 */
	~ResourceManager();

	/**
	 * @brief Gets the singleton instance for the resource manager
	 * @return Singleton instance of this class
	 */
	static ResourceManager& GetInstance();

	/**
	 * @brief Creates a shader program from the provided paths for the vertex and fragment shader files,
	 * and stores it as a shader resource identified by the provided string key.
	 * @param[in] vertexShaderFilePath File path to the vertex shader
	 * @param[in] fragmentShaderFilePath File path to the fragment shader
	 * @param[in] key Key identifier for the created shader program
	 * @return Reference to the created shader. Returns nullptr if the shader creation failed
	 */
	Engine::ShaderProgram* CreateShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& key);

	/**
	 * @brief Deletes the shader identified by the provided key from the resource manager
	 * @param[in] key Key identifying the shader to delete
	 */
	void DeleteShader(const std::string& key);

	/**
	 * @brief Gets the shader associated with the provided key.
	 * @param[in] key Key identifying the shader we are trying to get
	 * @return Returns the reference to the shader identified by the provided key. Returns nullptr if it does not exist
	 */
	Engine::ShaderProgram* GetShader(const std::string& key);

	/**
	 * @brief Creates a texture from the provided file path, and stores it
	 * as a texture resource identified by the provided string key.
	 * @param[in] textureFilePath File path to the texture file
	 * @param[in] key Key identifier for the created texture
	 * @return Reference to the created texture. Returns nullptr if the texture creation failed
	 */
	Engine::Texture* CreateTexture(const std::string& textureFilePath, const std::string& key);

	/**
	 * @brief Deletes the texture identified by the provided key from the resource manager
	 * @param[in] key Key idnetifying the texture to delete
	 */
	void DeleteTexture(const std::string& key);

	/**
	 * @brief Gets the texture associated with the provided key.
	 * @param[in] key Key identifying the texture we are trying to get
	 * @return Returns the reference to the texture identified by the provided key. Returns nullptr if it does not exist
	 */
	Engine::Texture* GetTexture(const std::string& key);
};
