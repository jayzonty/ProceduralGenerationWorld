#include "Shader.hpp"

#include <fstream>
#include <iostream>

/// <summary>
/// Constructor
/// </summary>
ShaderProgram::ShaderProgram()
	: m_program(0)
{
}

/// <summary>
/// Destructor
/// </summary>
ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
}

/// <summary>
/// Initialize the shader program from the provided
/// vertex and fragment shader file paths.
/// </summary>
/// <param name="vertexShaderFilePath">Vertex shader file path</param>
/// <param name="fragmentShaderFilePath">Fragment shader file path</param>
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
	if (linkStatus != GL_TRUE) {
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetProgramInfoLog(m_program, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "program link error: " << infoLog << std::endl;
	}
}

/// <summary>
/// Use the shader
/// </summary>
void ShaderProgram::Use()
{
	glUseProgram(m_program);
}

/// <summary>
/// Unuse the shader
/// </summary>
void ShaderProgram::Unuse()
{
	glUseProgram(0);
}

// ================
// --- Uniforms ---
// ================

/// <summary>
/// Sets the uniform value with a matrix
/// </summary>
/// <param name="uniformName">Name of the uniform</param>
/// <param name="transpose">Should the matrix be transformed?</param>
/// <param name="value">Pointer to the values in the matrix</param>
void ShaderProgram::SetUniformMatrix4fv(const std::string& uniformName, bool transpose, const float* value)
{
	GLint uniformLocation = glGetUniformLocation(m_program, uniformName.c_str());
	glUniformMatrix4fv(uniformLocation, 1, transpose ? GL_TRUE : GL_FALSE, value);
}

/// <summary>
/// Creates a shader object from the provided shader type and shader file
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="filePath">Shader file path</param>
/// <returns>Shader object handle</returns>
GLuint ShaderProgram::CreateShaderFromFile(const GLuint& shaderType, const std::string& filePath)
{
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail())
	{
		std::cerr << "Unable to open shader file: " << filePath << std::endl;
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

/// <summary>
/// Creates a shader object from the provided shader type and source
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="source">Shader source</param>
/// <returns>Shader object handle</returns>
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
		std::cerr << "shader compilation error: " << infoLog << std::endl;
	}

	return shader;
}
