#pragma once

#include <glad/glad.h>

#include <string>

/// <summary>
/// Texture class
/// </summary>
class Texture
{
private:
	/// <summary>
	/// GL texture handle
	/// </summary>
	GLuint m_textureHandle;

	/// <summary>
	/// Texture width
	/// </summary>
	int m_width;

	/// <summary>
	/// Texture height
	/// </summary>
	int m_height;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	Texture();

	/// <summary>
	/// Destructor
	/// </summary>
	~Texture();

	/// <summary>
	/// Create a texture based on the iamge in the provided file path
	/// </summary>
	/// <param name="textureFilePath">Image file path</param>
	void CreateFromFile(const std::string& textureFilePath);

	/// <summary>
	/// Gets the GL texture handle for this texture
	/// </summary>
	/// <returns>GL texture handle</returns>
	GLuint GetHandle() const;

};
