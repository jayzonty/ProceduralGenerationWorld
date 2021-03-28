#include "ResourceManager.hpp"

/// <summary>
	/// Constructor
	/// </summary>
ResourceManager::ResourceManager()
	: m_shaders()
	, m_textures()
{
}

/// <summary>
/// Constructor
/// </summary>
ResourceManager::~ResourceManager()
{
	for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		delete it->second;
	}
	m_shaders.clear();

	for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		delete it->second;
	}
	m_textures.clear();
}

/// <summary>
/// Gets the singleton instance for the resource manager
/// </summary>
/// <returns>Singleton instance of this clss</returns>
ResourceManager& ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return instance;
}

/// <summary>
/// Creates a shader program from the provided paths for the vertex and fragment shader files,
/// and stores it as a shader resource identified by the provided string key.
/// If the key already exists in the resource manager, shader creation will fail.
/// </summary>
/// <param name="vertexShaderFilePath">File path to the vertex shader</param>
/// <param name="fragmentShaderFilePath">File path to the fragment shader</param>
/// <param name="key">Key identifier for the created shader program</param>
/// <returns>Reference to the created shader. Returns nullptr if the shader creation failed.</returns>
ShaderProgram* ResourceManager::CreateShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& key)
{
	if (m_shaders.find(key) != m_shaders.end())
	{
		return nullptr;
	}

	ShaderProgram* shader = new ShaderProgram();
	shader->InitFromFiles(vertexShaderFilePath, fragmentShaderFilePath);
	m_shaders[key] = shader;

	return shader;
}

/// <summary>
/// Deletes the shader identified by the provided key from the resource manager
/// </summary>
/// <param name="key">Key identifying the shader to delete</param>
void ResourceManager::DeleteShader(const std::string& key)
{
	if (m_shaders.find(key) != m_shaders.end())
	{
		delete m_shaders[key];
		m_shaders.erase(key);
	}
}

/// <summary>
/// Gets the shader associated with the provided key.
/// </summary>
/// <param name="key">Key identifying the shader we are trying to get</param>
/// <returns>Returns the reference to the shader identified by the provided key. Returns nullptr if it does not exist.</returns>
ShaderProgram* ResourceManager::GetShader(const std::string& key)
{
	if (m_shaders.find(key) != m_shaders.end())
	{
		return m_shaders[key];
	}

	return nullptr;
}

/// <summary>
	/// Creates a texture from the provided file path, and stores it
	/// as a texture resource identified by the provided string key.
	/// If the key already exists in the resource manager, texture creation fails.
	/// </summary>
	/// <param name="textureFilePath">File path to the texture file</param>
	/// <param name="key">Key identifier for the created texture</param>
	/// <returns>Reference to the created texture. Returns nullptr if the texture creation failed.</returns>
Texture* ResourceManager::CreateTexture(const std::string& textureFilePath, const std::string& key)
{
	if (m_textures.find(key) != m_textures.end())
	{
		return nullptr;
	}

	Texture* texture = new Texture();
	texture->CreateFromFile(textureFilePath);
	m_textures[key] = texture;

	return texture;
}

/// <summary>
/// Deletes the texture identified by the provided key from the resource manager
/// </summary>
/// <param name="key">Key idnetifying the texture to delete</param>
void ResourceManager::DeleteTexture(const std::string& key)
{
	if (m_textures.find(key) != m_textures.end())
	{
		delete m_textures[key];
		m_textures.erase(key);
	}
}

/// <summary>
/// Gets the texture associated with the provided key.
/// </summary>
/// <param name="key">Key identifying the texture we are trying to get</param>
/// <returns>Returns the reference to the texture identified by the provided key. Returns nullptr if it does not exist.</returns>
Texture* ResourceManager::GetTexture(const std::string& key)
{
	if (m_textures.find(key) != m_textures.end())
	{
		return m_textures[key];
	}

	return nullptr;
}
