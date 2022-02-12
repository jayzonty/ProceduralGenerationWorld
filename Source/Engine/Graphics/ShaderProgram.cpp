#include "Engine/Graphics/ShaderProgram.hpp"

#include <fstream>
#include <iostream>

namespace Engine
{
/**
 * @brief Constructor
 */
ShaderProgram::ShaderProgram()
	: m_program(0)
{
}

/**
 * @brief Destructor
 */
ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
}

/**
 * @brief Initializes the shader program from the provided vertex and fragment shader file paths.
 * @param[in] vertexShaderFilePath Vertex shader file path
 * @param[in] fragmentShaderFilePath Fragment shader file path
 */
void ShaderProgram::InitFromFiles(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	GLuint vertexShader = CreateShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilePath);
	GLuint fragmentShader = CreateShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilePath);

	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);

	glLinkProgram(m_program);

	glDetachShader(m_program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(m_program, fragmentShader);
	glDeleteShader(fragmentShader);

	// Check shader program link status
	GLint linkStatus;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
    {
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetProgramInfoLog(m_program, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "[ShaderProgram] Program link error: " << infoLog << std::endl;
	}
}

/**
 * @brief Use the shader
 */
void ShaderProgram::Use()
{
	glUseProgram(m_program);
}

/**
 * @brief Unuses the shader
 */
void ShaderProgram::Unuse()
{
	glUseProgram(0);
}

// ================
// --- Uniforms ---
// ================

/**
 * @brief Sets the uniform value with a single integer
 * @param[in] uniformName Uniform name
 * @param[in] val Value
 */
void ShaderProgram::SetUniform1i(const std::string& uniformName, const int& val)
{
	GLint uniformLocation = glGetUniformLocation(m_program, uniformName.c_str());
	glUniform1i(uniformLocation, val);
}

/**
 * @brief Sets the uniform value with a single float
 * @param[in] uniformName Uniform name
 * @param[in] val Value
 */
void ShaderProgram::SetUniform1f(const std::string& uniformName, const float& val)
{
	GLint uniformLocation = glGetUniformLocation(m_program, uniformName.c_str());
	glUniform1f(uniformLocation, val);
}

/**
 * @brief Sets the uniform value with 3 floats
 * @param[in] uniformName Uniform name
 * @param[in] val1 First value
 * @param[in] val2 Second value
 * @param[in] val3 Third value
 */
void ShaderProgram::SetUniform3f(const std::string& uniformName, const float& val1, const float& val2, const float& val3)
{
	GLint uniformLocation = glGetUniformLocation(m_program, uniformName.c_str());
	glUniform3f(uniformLocation, val1, val2, val3);
}

/**
 * @brief Sets the uniform value with 4 floats
 * @param[in] uniformName Uniform name
 * @param[in] val1 First value
 * @param[in] val2 Second value
 * @param[in] val3 Third value
 * @param[in] val4 Fourth value
 */
void ShaderProgram::SetUniform4f(const std::string& uniformName, const float& val1, const float& val2, const float& val3, const float& val4)
{
	GLint uniformLocation = glGetUniformLocation(m_program, uniformName.c_str());
	glUniform4f(uniformLocation, val1, val2, val3, val4);
}

/**
 * @brief Sets the uniform value with a matrix
 * @param[in] uniformName Name of the uniform
 * @param[in] transpose Flag specifying whether the matrix should be transposed or not.
 * @param[in] value Pointer to the values in the matrix
 */
void ShaderProgram::SetUniformMatrix4fv(const std::string& uniformName, bool transpose, const float* value)
{
	GLint uniformLocation = glGetUniformLocation(m_program, uniformName.c_str());
	glUniformMatrix4fv(uniformLocation, 1, transpose ? GL_TRUE : GL_FALSE, value);
}

/**
 * @brief Creates a shader object from the provided shader type and shader file
 * @param[in] shaderType Shader type
 * @param[in] filePath Shader file path
 * @return Shader object handle
 */
GLuint ShaderProgram::CreateShaderFromFile(const GLuint& shaderType, const std::string& filePath)
{
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail())
	{
		std::cerr << "[ShaderProgram] Unable to open shader file: " << filePath << std::endl;
		return 0;
	}

	std::string shaderSource;
	std::string temp;
	while (std::getline(shaderFile, temp))
	{
		shaderSource += temp + "\n";
	}
	shaderFile.close();

	return CreateShaderFromSource(shaderType, shaderSource);
}

/**
 * @brief Creates a shader object from the provided shader type and source
 * @param[in] shaderType Shader type
 * @param[in] source Shader source
 * @return Shader object handle
 */
GLuint ShaderProgram::CreateShaderFromSource(const GLuint& shaderType, const std::string& source)
{
	GLuint shader = glCreateShader(shaderType);

	const char* shaderSourceCStr = source.c_str();
	GLint shaderSourceLen = static_cast<GLint>(source.length());
	glShaderSource(shader, 1, &shaderSourceCStr, &shaderSourceLen);
	glCompileShader(shader);

	// Check compilation status
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetShaderInfoLog(shader, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "[ShaderProgram] Shader compilation error: " << infoLog << std::endl;
	}

	return shader;
}
}
