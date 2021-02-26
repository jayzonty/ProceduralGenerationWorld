#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

/// <summary>
/// Font class
/// </summary>
class Font
{
public:
	/// <summary>
	/// Glyph metrics struct
	/// </summary>
	struct GlyphMetrics
	{
		/// <summary>
		/// Glyph width
		/// </summary>
		int width;

		/// <summary>
		/// Glyph height
		/// </summary>
		int height;

		/// <summary>
		/// Bearing (X)
		/// </summary>
		int bearingX;

		/// <summary>
		/// Bearing (Y)
		/// </summary>
		int bearingY;

		/// <summary>
		/// Advance amount
		/// </summary>
		int advance;
	};

private:
	friend class Text;

	/// <summary>
	/// Font size
	/// </summary>
	int m_fontSize;

	/// <summary>
	/// List of glyph metrics
	/// </summary>
	std::vector<GlyphMetrics> m_glyphMetrics;

	/// <summary>
	/// Array of texture objects for each glyph
	/// </summary>
	GLuint m_glyphTextures[128];

	/// <summary>
	/// Flag indicating whether the font was loaded or not
	/// </summary>
	bool m_isLoaded;

	/// <summary>
	/// Path to the font file
	/// </summary>
	std::string m_fontFilePath;

public:

	/// <summary>
	/// Constructor
	/// </summary>
	Font();

	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="other">Font object to copy from</param>
	Font(const Font& other);

	/// <summary>
	/// Destructor
	/// </summary>
	~Font();

	/// <summary>
	/// Load the font from the given filename
	/// </summary>
	/// <param name="fontFilePath"></param>
	/// <returns>Returns true if loading was successful. Returns false otherwise.</returns>
	bool Load(const char* fontFilePath);

	/// <summary>
	/// Query whether the font has already been loaded or not.
	/// </summary>
	/// <returns>Returns true if the font has been loaded. Returns false otherwise.</returns>
	bool IsLoaded() const;

	/// <summary>
	/// Sets the font size
	/// </summary>
	/// <param name="fontSize">New font size</param>
	void SetSize(int fontSize);

	/// <summary>
	/// Gets the glyph metrics for the provided character
	/// </summary>
	/// <param name="c">Character whose glyph metrics we are querying</param>
	/// <returns>Glyph metrics for the provided character.</returns>
	const GlyphMetrics* GetGlyphMetrics(char c);

private:
	/// <summary>
	/// Gets the texture id for the provided character
	/// </summary>
	/// <param name="c">Character whose texture ID we are querying.</param>
	/// <returns>Texture ID for the provided character.</returns>
	GLuint GetGlyphTextureId(char c);
};
