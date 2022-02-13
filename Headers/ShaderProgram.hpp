#pragma once

#include <glad/glad.h>

#include <string>

class ShaderProgram
{
private:
    /**
     * Shader program handle
     */
	GLuint m_program;

public:
    /**
     * @brief Constructor
     */
	ShaderProgram();

    /**
     * @brief Destructor
     */
	~ShaderProgram();

    /**
     * @brief Initializes the shader program from the provided vertex and fragment shader file paths.
     * @param[in] vertexShaderFilePath Vertex shader file path
     * @param[in] fragmentShaderFilePath Fragment shader file path
     */
	void InitFromFiles(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

    /**
     * @brief Use the shader
     */
	void Use();

    /**
     * @brief Unuses the shader
     */
	void Unuse();

	// ================
	// --- Uniforms ---
	// ================

    /**
     * @brief Sets the uniform value with a single integer
     * @param[in] uniformName Uniform name
     * @param[in] val Value
     */
	void SetUniform1i(const std::string& uniformName, const int& val);

    /**
     * @brief Sets the uniform value with a single float
     * @param[in] uniformName Uniform name
     * @param[in] val Value
     */
	void SetUniform1f(const std::string& uniformName, const float& val);

    /**
     * @brief Sets the uniform value with 3 floats
     * @param[in] uniformName Uniform name
     * @param[in] val1 First value
     * @param[in] val2 Second value
     * @param[in] val3 Third value
     */
	void SetUniform3f(const std::string& uniformName, const float& val1, const float& val2, const float& val3);

    /**
     * @brief Sets the uniform value with 4 floats
     * @param[in] uniformName Uniform name
     * @param[in] val1 First value
     * @param[in] val2 Second value
     * @param[in] val3 Third value
     * @param[in] val4 Fourth value
     */
	void SetUniform4f(const std::string& uniformName, const float& val1, const float& val2, const float& val3, const float& val4);

    /**
     * @brief Sets the uniform value with a matrix
     * @param[in] uniformName Name of the uniform
     * @param[in] transpose Flag specifying whether the matrix should be transposed or not.
     * @param[in] value Pointer to the values in the matrix
     */
	void SetUniformMatrix4fv(const std::string& uniformName, bool transpose, const float* value);

private:

    /**
     * @brief Creates a shader object from the provided shader type and shader file
     * @param[in] shaderType Shader type
     * @param[in] filePath Shader file path
     * @return Shader object handle
     */
	GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& filePath);

    /**
     * @brief Creates a shader object from the provided shader type and source
     * @param[in] shaderType Shader type
     * @param[in] source Shader source
     * @return Shader object handle
     */
	GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& source);
};
