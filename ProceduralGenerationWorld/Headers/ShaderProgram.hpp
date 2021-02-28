#pragma once

#include <glad/glad.h>

#include <string>

class ShaderProgram
{
private:
	/// <summary>
	/// Shader program handle
	/// </summary>
	GLuint m_program;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	ShaderProgram();

	/// <summary>
	/// Destructor
	/// </summary>
	~ShaderProgram();

	/// <summary>
	/// Initializes the shader program from the provided
	/// vertex and fragment shader file paths.
	/// </summary>
	/// <param name="vertexShaderFilePath">Vertex shader file path</param>
	/// <param name="fragmentShaderFilePath">Fragment shader file path</param>
	void InitFromFiles(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	/// <summary>
	/// Uses the shader
	/// </summary>
	void Use();

	/// <summary>
	/// Unuses the shader
	/// </summary>
	void Unuse();

	// ================
	// --- Uniforms ---
	// ================

	/// <summary>
	/// Sets the uniform value with a single integer
	/// </summary>
	/// <param name="uniformName">Uniform name</param>
	/// <param name="val">Value</param>
	void SetUniform1i(const std::string& uniformName, const int& val);

	/// <summary>
	/// Sets the uniform value with a single float
	/// </summary>
	/// <param name="uniformName">Uniform name</param>
	/// <param name="val">Value</param>
	void SetUniform1f(const std::string& uniformName, const float& val);

	/// <summary>
	/// Sets the uniform value with 3 floats
	/// </summary>
	/// <param name="uniformName">Uniform name</param>
	/// <param name="val1">First value</param>
	/// <param name="val2">Second value</param>
	/// <param name="val3">Third value</param>
	void SetUniform3f(const std::string& uniformName, const float& val1, const float& val2, const float& val3);

	/// <summary>
	/// Sets the uniform value with 4 floats
	/// </summary>
	/// <param name="uniformName">Uniform name</param>
	/// <param name="val1">First value</param>
	/// <param name="val2">Second value</param>
	/// <param name="val3">Third value</param>
	/// <param name="val4">Fourth value</param
	void SetUniform4f(const std::string& uniformName, const float& val1, const float& val2, const float& val3, const float& val4);

	/// <summary>
	/// Sets the uniform value with a matrix
	/// </summary>
	/// <param name="uniformName">Name of the uniform</param>
	/// <param name="transpose">Should the matrix be transformed?</param>
	/// <param name="value">Pointer to the values in the matrix</param>
	void SetUniformMatrix4fv(const std::string& uniformName, bool transpose, const float* value);

private:

	/// <summary>
	/// Creates a shader object from the provided shader type and shader file
	/// </summary>
	/// <param name="shaderType">Shader type</param>
	/// <param name="filePath">Shader file path</param>
	/// <returns>Shader object handle</returns>
	GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& filePath);

	/// <summary>
	/// Creates a shader object from the provided shader type and source
	/// </summary>
	/// <param name="shaderType">Shader type</param>
	/// <param name="source">Shader source</param>
	/// <returns>Shader object handle</returns>
	GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& source);

};
