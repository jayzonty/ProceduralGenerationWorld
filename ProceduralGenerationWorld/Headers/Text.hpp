#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstdio>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Font.hpp"

#include <vector>

/// <summary>
/// Text display class
/// </summary>
class Text
{
public:
	/// <summary>
	/// Horizontal alignment enum
	/// </summary>
	enum class HorizontalAlignment
	{
		LEFT,
		CENTER,
		RIGHT
	};

private:
	/// <summary>
	/// Struct containing info for a line of text
	/// </summary>
	struct Line {
		/// <summary>
		/// String
		/// </summary>
		std::string str;

		/// <summary>
		/// Line width
		/// </summary>
		int width;
	};

	/// <summary>
	/// VBO
	/// </summary>
	GLuint m_vbo;

	/// <summary>
	/// VAO
	/// </summary>
	GLuint m_vao;

	/// <summary>
	/// Reference to the font
	/// </summary>
	Font* m_font;

	/// <summary>
	/// Shader
	/// </summary>
	ShaderProgram m_shader;

	/// <summary>
	/// Text color
	/// </summary>
	glm::vec4 m_color;

	/// <summary>
	/// List of line info
	/// </summary>
	std::vector<Line> m_lines;

	/// <summary>
	/// Amount of line spacing
	/// </summary>
	float m_lineSpacing;

	/// <summary>
	/// Line height
	/// </summary>
	int m_lineHeight;

	/// <summary>
	/// X-position
	/// </summary>
	float m_x;

	/// <summary>
	/// Y-position
	/// </summary>
	float m_y;

	/// <summary>
	/// Horizontal alignment
	/// </summary>
	HorizontalAlignment m_horizontalAlign;

	/// <summary>
	/// Width
	/// </summary>
	int m_width;

	/// <summary>
	/// Height
	/// </summary>
	int m_height;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	Text();

	/// <summary>
	/// Destructor
	/// </summary>
	~Text();

	/// <summary>
	/// Sets the line spacing
	/// </summary>
	/// <param name="lineSpacing">Line spacing</param>
	void SetLineSpacing(float lineSpacing);

	/// <summary>
	/// Sets the text font
	/// </summary>
	/// <param name="font">Font</param>
	void SetFont(Font& font);

	/// <summary>
	/// Sets the string of text to display
	/// </summary>
	/// <param name="str">String to display</param>
	void SetString(const std::string& str);

	/// <summary>
	/// Sets the text color
	/// </summary>
	/// <param name="color">Text color</param>
	void SetColor(const glm::vec4& color);

	/// <summary>
	/// Sets the position of the text
	/// </summary>
	/// <param name="x">X-position</param>
	/// <param name="y">Y-position</param>
	void SetPosition(float x, float y);

	/// <summary>
	/// Gets the text position
	/// </summary>
	/// <returns>Text position</returns>
	glm::vec2 GetPosition() const;

	/// <summary>
	/// Draws the text
	/// </summary>
	/// <param name="projectionMatrix">Projection matrix</param>
	void Draw(const glm::mat4& projectionMatrix);

	/// <summary>
	/// Computes the size of the bounds of the text
	/// </summary>
	/// <param name="width">Variable to set the width to</param>
	/// <param name="height">Variable to set the height to</param>
	void ComputeSize(int* width, int* height);

	/// <summary>
	/// Sets the horizontal alignment of the text
	/// </summary>
	/// <param name="horizontalAlign">Horizontal alignment</param>
	void SetHorizontalAlignment(HorizontalAlignment horizontalAlign);
};
