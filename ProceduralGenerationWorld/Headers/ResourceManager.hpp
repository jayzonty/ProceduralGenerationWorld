#pragma once

#include "ShaderProgram.hpp"

#include <map>
#include <string>

/// <summary>
/// Resource manager class
/// </summary>
class ResourceManager
{
private:

	/// <summary>
	/// List of shaders identified by their corresponding string keys
	/// </summary>
	std::map<std::string, ShaderProgram*> m_shaders;

	/// <summary>
	/// Constructor
	/// </summary>
	ResourceManager();

	/* Delete copy constructor */
	ResourceManager(const ResourceManager&) = delete;

	/* Delete copy operator */
	ResourceManager& operator=(const ResourceManager&) = delete;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	~ResourceManager();

	/// <summary>
	/// Gets the singleton instance for the resource manager
	/// </summary>
	/// <returns>Singleton instance of this clss</returns>
	static ResourceManager& GetInstance();

	/// <summary>
	/// Creates a shader program from the provided paths for the vertex and fragment shader files,
	/// and stores it as a shader resource identified by the provided string key.
	/// </summary>
	/// <param name="vertexShaderFilePath">File path to the vertex shader</param>
	/// <param name="fragmentShaderFilePath">File path to the fragment shader</param>
	/// <param name="key">Key identifier for the created shader program</param>
	/// <returns>Reference to the created shader. Returns nullptr if the shader creation failed.</returns>
	ShaderProgram* CreateShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::string& key);

	/// <summary>
	/// Deletes the shader identified by the provided key from the resource manager
	/// </summary>
	/// <param name="key">Key identifying the shader to delete</param>
	void DeleteShader(const std::string& key);

	/// <summary>
	/// Gets the shader associated with the provided key.
	/// </summary>
	/// <param name="key">Key identifying the shader we are trying to get</param>
	/// <returns>Returns the reference to the shader identified by the provided key. Returns nullptr if it does not exist.</returns>
	ShaderProgram* GetShader(const std::string& key);

};
